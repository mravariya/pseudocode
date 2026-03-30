#include "pc_interp.h"
#include "pc_common.h"
#include "pc_stdlib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Sym {
  char *name;
  PcValue val;
  bool is_const;
  struct Sym *alias_of;
  struct Sym *next;
} Sym;

typedef struct Env {
  Sym *syms;
  struct Env *parent;
} Env;

typedef struct NamedAst {
  char *name;
  PcAst *def;
  struct NamedAst *next;
} NamedAst;

typedef struct OpenFile {
  char *path;
  FILE *fp;
  int mode; /* 0 read 1 write 2 append 3 random */
} OpenFile;

struct Interp {
  PcErrorCtx *err;
  Env *global;
  Env *env;
  NamedAst *procs;
  NamedAst *funcs;
  OpenFile *files;
  size_t file_count;
  PcValue ret;
  bool returned;
  bool trace;
};

static Sym *sym_find_chain(Env *e, const char *name) {
  for (; e; e = e->parent) {
    for (Sym *s = e->syms; s; s = s->next) {
      if (pc_str_eq_ci(s->name, name)) return s;
    }
  }
  return NULL;
}

static Sym *sym_find(Interp *I, const char *name) { return sym_find_chain(I->env, name); }

static Sym *sym_find_local(Env *e, const char *name) {
  if (!e) return NULL;
  for (Sym *s = e->syms; s; s = s->next) {
    if (pc_str_eq_ci(s->name, name)) return s;
  }
  return NULL;
}

static void sym_put(Env *e, Sym *s) {
  s->next = e->syms;
  e->syms = s;
}

static Env *env_new(Env *parent) {
  Env *e = calloc(1, sizeof(Env));
  e->parent = parent;
  return e;
}

static void sym_free_list(Sym *s) {
  while (s) {
    Sym *n = s->next;
    free(s->name);
    if (!s->alias_of) pc_value_destroy(&s->val);
    free(s);
    s = n;
  }
}

static void env_free(Env *e) {
  if (!e) return;
  sym_free_list(e->syms);
  free(e);
}

static NamedAst *named_find(NamedAst *h, const char *name) {
  for (; h; h = h->next) {
    if (pc_str_eq_ci(h->name, name)) return h;
  }
  return NULL;
}

Interp *pc_interp_new(PcErrorCtx *err) {
  Interp *I = calloc(1, sizeof(Interp));
  I->err = err;
  I->global = env_new(NULL);
  I->env = I->global;
  I->trace = false;
  return I;
}

void pc_interp_set_trace(Interp *I, bool on) {
  if (I) I->trace = on;
}

static void close_all_files(Interp *I) {
  for (size_t i = 0; i < I->file_count; i++) {
    if (I->files[i].fp) fclose(I->files[i].fp);
    free(I->files[i].path);
  }
  free(I->files);
  I->files = NULL;
  I->file_count = 0;
}

void pc_interp_free(Interp *I) {
  if (!I) return;
  close_all_files(I);
  env_free(I->global);
  for (NamedAst *n = I->procs; n;) {
    NamedAst *x = n->next;
    free(n->name);
    free(n);
    n = x;
  }
  for (NamedAst *n = I->funcs; n;) {
    NamedAst *x = n->next;
    free(n->name);
    free(n);
    n = x;
  }
  pc_value_destroy(&I->ret);
  free(I);
}

static int eval_int(Interp *I, PcAst *e);
static int64_t to_int_compat(const PcValue *v);
static Sym *resolve_array_sym(Interp *I, PcAst *e);
static void eval_expr(Interp *I, PcAst *e, PcValue *out);
static void exec_stmt(Interp *I, PcAst *s);
static void exec_block(Interp *I, PcAst *s);

static FILE *find_file_fp(Interp *I, const char *path) {
  for (size_t i = 0; i < I->file_count; i++) {
    if (I->files[i].path && strcmp(I->files[i].path, path) == 0) return I->files[i].fp;
  }
  return NULL;
}

static void file_set(Interp *I, const char *path, FILE *fp, int mode) {
  for (size_t i = 0; i < I->file_count; i++) {
    if (I->files[i].path && strcmp(I->files[i].path, path) == 0) {
      if (I->files[i].fp) fclose(I->files[i].fp);
      I->files[i].fp = fp;
      I->files[i].mode = mode;
      return;
    }
  }
  size_t n = I->file_count + 1;
  I->files = realloc(I->files, n * sizeof(OpenFile));
  I->files[I->file_count].path = pc_strdup(path);
  I->files[I->file_count].fp = fp;
  I->files[I->file_count].mode = mode;
  I->file_count = n;
}

static bool eof_file(Interp *I, const char *path) {
  FILE *f = find_file_fp(I, path);
  if (!f) return true;
  int c = fgetc(f);
  if (c == EOF) return true;
  ungetc(c, f);
  return false;
}

static int eval_int(Interp *I, PcAst *e) {
  PcValue v;
  eval_expr(I, e, &v);
  int x = 0;
  if (v.type && v.type->kind == PC_T_INT) x = (int)v.i;
  else if (v.type && v.type->kind == PC_T_REAL) x = (int)v.r;
  pc_value_destroy(&v);
  return x;
}

static int64_t to_int_compat(const PcValue *v) {
  if (!v->type) return 0;
  if (v->type->kind == PC_T_INT) return v->i;
  if (v->type->kind == PC_T_REAL) return (int64_t)v->r;
  return 0;
}

static Sym *resolve_array_sym(Interp *I, PcAst *e) {
  PcAst *v = e;
  while (v && v->kind == PC_AST_INDEX) v = v->left;
  if (!v || v->kind != PC_AST_VAR) return NULL;
  return sym_find(I, v->name);
}

static void eval_expr(Interp *I, PcAst *e, PcValue *out) {
  if (!e) {
    pc_value_init_void(out);
    return;
  }
  PcSourceLoc loc = e->loc;
  switch (e->kind) {
  case PC_AST_LITERAL: {
    if (e->lit_is_bool) {
      pc_value_init_bool(out, e->bool_val);
      return;
    }
    if (e->lit_is_char) {
      pc_value_init_char(out, e->char_val);
      return;
    }
    if (e->lit_is_real) {
      pc_value_init_real(out, e->real_val);
      return;
    }
    if (e->str_val) {
      if (strchr(e->str_val, '/'))
        pc_value_init_date(out, e->str_val);
      else
        pc_value_init_string(out, pc_strdup(e->str_val), true);
      return;
    }
    pc_value_init_int(out, e->int_val);
    return;
  }
  case PC_AST_VAR: {
    Sym *s = sym_find(I, e->name);
    if (!s) {
      pc_runtime_error(I->err, loc, "undefined variable '%s'", e->name);
      pc_value_init_void(out);
      return;
    }
    Sym *t = s->alias_of ? s->alias_of : s;
    pc_value_copy(out, &t->val);
    return;
  }
  case PC_AST_INDEX: {
    Sym *as = resolve_array_sym(I, e);
    if (!as || !as->val.type || as->val.type->kind != PC_T_ARRAY || !as->val.cells) {
      pc_runtime_error(I->err, loc, "indexed value is not an array");
      pc_value_init_void(out);
      return;
    }
    PcType *t = as->val.type;
    PcValue *cells = as->val.cells;
    if (!t->is2d) {
      int i1 = eval_int(I, e->child);
      if (i1 < t->lo0 || i1 > t->hi0) {
        pc_runtime_error(I->err, loc, "array index out of range");
        pc_value_init_void(out);
        return;
      }
      pc_value_copy(out, &cells[i1 - t->lo0]);
      return;
    }
    if (e->left->kind != PC_AST_INDEX) {
      pc_runtime_error(I->err, loc, "invalid 2D index");
      pc_value_init_void(out);
      return;
    }
    int row = eval_int(I, e->left->child);
    int col = eval_int(I, e->child);
    if (row < t->lo0 || row > t->hi0 || col < t->lo1 || col > t->hi1) {
      pc_runtime_error(I->err, loc, "array index out of range");
      pc_value_init_void(out);
      return;
    }
    int stride = t->hi1 - t->lo1 + 1;
    int idx = (row - t->lo0) * stride + (col - t->lo1);
    pc_value_copy(out, &cells[idx]);
    return;
  }
  case PC_AST_CALL_EXPR: {
    if (pc_str_eq_ci(e->name, "EOF") && e->arg_count == 1) {
      PcValue p;
      eval_expr(I, e->args[0], &p);
      const char *path = p.s ? p.s : "";
      pc_value_init_bool(out, eof_file(I, path));
      pc_value_destroy(&p);
      return;
    }
    PcValue *argv = calloc(e->arg_count, sizeof(PcValue));
    for (size_t i = 0; i < e->arg_count; i++) {
      eval_expr(I, e->args[i], &argv[i]);
    }
    if (pc_stdlib_try_call(I->err, e->name, argv, e->arg_count, out, loc)) {
      for (size_t i = 0; i < e->arg_count; i++) pc_value_destroy(&argv[i]);
      free(argv);
      return;
    }
    NamedAst *nf = named_find(I->funcs, e->name);
    if (!nf) {
      pc_runtime_error(I->err, loc, "undefined function '%s'", e->name);
      for (size_t i = 0; i < e->arg_count; i++) pc_value_destroy(&argv[i]);
      free(argv);
      pc_value_init_void(out);
      return;
    }
    PcAst *fn = nf->def;
    Env *local = env_new(I->global);
    Env *saved = I->env;
    I->env = local;
    for (size_t i = 0; i < fn->param_count; i++) {
      Sym *s = calloc(1, sizeof(Sym));
      s->name = pc_strdup(fn->params[i].name);
      pc_value_copy(&s->val, &argv[i]);
      sym_put(local, s);
    }
    for (size_t i = 0; i < e->arg_count; i++) pc_value_destroy(&argv[i]);
    free(argv);
    I->returned = false;
    for (PcAst *st = fn->body; st; st = st->next) {
      exec_stmt(I, st);
      if (I->returned) break;
    }
    if (!I->returned) {
      pc_runtime_error(I->err, loc, "function '%s' did not RETURN", e->name);
    }
    pc_value_copy(out, &I->ret);
    pc_value_destroy(&I->ret);
    I->returned = false;
    env_free(local);
    I->env = saved;
    return;
  }
  case PC_AST_UNARY: {
    PcValue a;
    eval_expr(I, e->child, &a);
    if (e->op == TOK_NOT) {
      pc_value_init_bool(out, !pc_value_truthy(&a));
    } else if (e->op == TOK_MINUS) {
      if (a.type && a.type->kind == PC_T_INT)
        pc_value_init_int(out, -a.i);
      else if (a.type && a.type->kind == PC_T_REAL)
        pc_value_init_real(out, -a.r);
      else
        pc_value_init_int(out, -a.i);
    }
    pc_value_destroy(&a);
    return;
  }
  case PC_AST_BINARY: {
    PcValue Lv, Rv;
    eval_expr(I, e->left, &Lv);
    if (e->op == TOK_AND) {
      if (!pc_value_truthy(&Lv)) {
        pc_value_init_bool(out, false);
        pc_value_destroy(&Lv);
        return;
      }
      pc_value_destroy(&Lv);
      eval_expr(I, e->right, &Rv);
      pc_value_init_bool(out, pc_value_truthy(&Rv));
      pc_value_destroy(&Rv);
      return;
    }
    if (e->op == TOK_OR) {
      if (pc_value_truthy(&Lv)) {
        pc_value_init_bool(out, true);
        pc_value_destroy(&Lv);
        return;
      }
      pc_value_destroy(&Lv);
      eval_expr(I, e->right, &Rv);
      pc_value_init_bool(out, pc_value_truthy(&Rv));
      pc_value_destroy(&Rv);
      return;
    }
    eval_expr(I, e->right, &Rv);
    if (e->op == TOK_AMP || e->op == TOK_COMMA) {
      char *ls = NULL, *rs = NULL;
      if (Lv.type->kind == PC_T_STRING) ls = Lv.s ? Lv.s : "";
      if (Rv.type->kind == PC_T_STRING) rs = Rv.s ? Rv.s : "";
      if (!ls || !rs) {
        pc_runtime_error(I->err, loc,
                           e->op == TOK_AMP ? "& expects STRING operands"
                                            : "comma (,) joins strings here; both sides must be STRING");
        pc_value_destroy(&Lv);
        pc_value_destroy(&Rv);
        pc_value_init_void(out);
        return;
      }
      size_t la = strlen(ls), lb = strlen(rs);
      char *buf = malloc(la + lb + 1);
      memcpy(buf, ls, la);
      memcpy(buf + la, rs, lb + 1);
      pc_value_init_string(out, buf, true);
      pc_value_destroy(&Lv);
      pc_value_destroy(&Rv);
      return;
    }
    if (e->op == TOK_PLUS && Lv.type->kind == PC_T_STRING && Rv.type->kind == PC_T_STRING) {
      const char *ls = Lv.s ? Lv.s : "";
      const char *rs = Rv.s ? Rv.s : "";
      size_t la = strlen(ls), lb = strlen(rs);
      char *buf = malloc(la + lb + 1);
      memcpy(buf, ls, la);
      memcpy(buf + la, rs, lb + 1);
      pc_value_init_string(out, buf, true);
      pc_value_destroy(&Lv);
      pc_value_destroy(&Rv);
      return;
    }
    if (e->op == TOK_PLUS &&
        (Lv.type->kind == PC_T_STRING || Rv.type->kind == PC_T_STRING)) {
      pc_runtime_error(I->err, loc, "+ with STRING requires both operands to be STRING; use & or numeric +");
      pc_value_destroy(&Lv);
      pc_value_destroy(&Rv);
      pc_value_init_void(out);
      return;
    }
    double l = 0, r = 0;
    bool sint = Lv.type->kind == PC_T_INT && Rv.type->kind == PC_T_INT;
    if (Lv.type->kind == PC_T_INT) l = (double)Lv.i;
    else if (Lv.type->kind == PC_T_REAL) l = Lv.r;
    if (Rv.type->kind == PC_T_INT) r = (double)Rv.i;
    else if (Rv.type->kind == PC_T_REAL) r = Rv.r;
    switch (e->op) {
    case TOK_PLUS:
      if (sint)
        pc_value_init_int(out, (int64_t)(l + r));
      else
        pc_value_init_real(out, l + r);
      break;
    case TOK_MINUS:
      if (sint)
        pc_value_init_int(out, (int64_t)(l - r));
      else
        pc_value_init_real(out, l - r);
      break;
    case TOK_STAR:
      if (sint)
        pc_value_init_int(out, (int64_t)(l * r));
      else
        pc_value_init_real(out, l * r);
      break;
    case TOK_SLASH:
      pc_value_init_real(out, l / r);
      break;
    case TOK_DIV:
      if ((int64_t)r == 0) {
        pc_runtime_error(I->err, loc, "division by zero");
        pc_value_init_void(out);
      } else
        pc_value_init_int(out, (int64_t)l / (int64_t)r);
      break;
    case TOK_MOD:
      pc_value_init_int(out, (int64_t)l % (int64_t)r);
      break;
    case TOK_EQ:
    case TOK_NE:
    case TOK_LT:
    case TOK_GT:
    case TOK_LE:
    case TOK_GE: {
      bool res = false;
      if (Lv.type->kind == PC_T_STRING && Rv.type->kind == PC_T_STRING) {
        int c = strcmp(Lv.s ? Lv.s : "", Rv.s ? Rv.s : "");
        if (e->op == TOK_EQ) res = c == 0;
        if (e->op == TOK_NE) res = c != 0;
        if (e->op == TOK_LT) res = c < 0;
        if (e->op == TOK_GT) res = c > 0;
        if (e->op == TOK_LE) res = c <= 0;
        if (e->op == TOK_GE) res = c >= 0;
      } else {
        if (e->op == TOK_EQ) res = l == r;
        if (e->op == TOK_NE) res = l != r;
        if (e->op == TOK_LT) res = l < r;
        if (e->op == TOK_GT) res = l > r;
        if (e->op == TOK_LE) res = l <= r;
        if (e->op == TOK_GE) res = l >= r;
      }
      pc_value_init_bool(out, res);
      break;
    }
    default:
      pc_value_init_void(out);
      break;
    }
    pc_value_destroy(&Lv);
    pc_value_destroy(&Rv);
    return;
  }
  default:
    pc_value_init_void(out);
    return;
  }
}

static PcValue *lvalue_ptr(Interp *I, PcAst *e, PcSourceLoc loc) {
  if (e->kind == PC_AST_VAR) {
    Sym *s = sym_find(I, e->name);
    if (!s) {
      pc_runtime_error(I->err, loc, "undefined variable '%s'", e->name);
      return NULL;
    }
    return &(s->alias_of ? s->alias_of : s)->val;
  }
  if (e->kind == PC_AST_INDEX) {
    Sym *arr = resolve_array_sym(I, e);
    if (!arr || !arr->val.type || arr->val.type->kind != PC_T_ARRAY || !arr->val.cells) {
      pc_runtime_error(I->err, loc, "indexed target is not an array");
      return NULL;
    }
    PcType *t = arr->val.type;
    if (!t->is2d) {
      int i1 = eval_int(I, e->child);
      if (i1 < t->lo0 || i1 > t->hi0) {
        pc_runtime_error(I->err, loc, "array index out of range");
        return NULL;
      }
      return &arr->val.cells[i1 - t->lo0];
    }
    if (e->left->kind != PC_AST_INDEX) {
      pc_runtime_error(I->err, loc, "invalid 2D index");
      return NULL;
    }
    int row = eval_int(I, e->left->child);
    int col = eval_int(I, e->child);
    if (row < t->lo0 || row > t->hi0 || col < t->lo1 || col > t->hi1) {
      pc_runtime_error(I->err, loc, "array index out of range");
      return NULL;
    }
    int stride = t->hi1 - t->lo1 + 1;
    int idx = (row - t->lo0) * stride + (col - t->lo1);
    return &arr->val.cells[idx];
  }
  return NULL;
}

static void register_defs(Interp *I, PcAst *p) {
  for (PcAst *s = p; s; s = s->next) {
    if (s->kind == PC_AST_PROC) {
      NamedAst *n = calloc(1, sizeof(NamedAst));
      n->name = pc_strdup(s->name);
      n->def = s;
      n->next = I->procs;
      I->procs = n;
    } else if (s->kind == PC_AST_FUNC) {
      NamedAst *n = calloc(1, sizeof(NamedAst));
      n->name = pc_strdup(s->name);
      n->def = s;
      n->next = I->funcs;
      I->funcs = n;
    }
  }
}

static void declare_array(Interp *I, PcAst *d) {
  int lo = eval_int(I, d->for_start);
  int hi = eval_int(I, d->for_stop);
  PcAst *lo2a = d->for_step;
  PcAst *hi2a = d->child;
  PcType *t = d->vtype;
  if (!t || t->kind != PC_T_ARRAY) return;
  int n = (hi - lo + 1);
  int lo1 = 0, hi1 = 0;
  if (lo2a && hi2a) {
    lo1 = eval_int(I, lo2a);
    hi1 = eval_int(I, hi2a);
    n *= (hi1 - lo1 + 1);
  }
  if (n < 0 || n > 1000000) {
    pc_runtime_error(I->err, d->loc, "invalid array bounds");
    return;
  }
  t->lo0 = lo;
  t->hi0 = hi;
  t->is2d = lo2a && hi2a;
  if (t->is2d) {
    t->lo1 = lo1;
    t->hi1 = hi1;
  }
  PcValue *cells = calloc((size_t)n, sizeof(PcValue));
  PcType *el = t->elem;
  for (int i = 0; i < n; i++) {
    if (el->kind == PC_T_INT) pc_value_init_int(&cells[i], 0);
    else if (el->kind == PC_T_REAL) pc_value_init_real(&cells[i], 0.0);
    else if (el->kind == PC_T_BOOL) pc_value_init_bool(&cells[i], false);
    else if (el->kind == PC_T_CHAR) pc_value_init_char(&cells[i], ' ');
    else if (el->kind == PC_T_STRING) pc_value_init_string(&cells[i], pc_strdup(""), true);
    else if (el->kind == PC_T_DATE) {
      memset(&cells[i], 0, sizeof(cells[i]));
      cells[i].type = pc_type_clone(el);
      cells[i].s = pc_strdup("01/01/2000");
      cells[i].owned_str = true;
    }
    if (cells[i].type && el->kind != PC_T_DATE) cells[i].type->kind = el->kind;
  }
  Sym *s = calloc(1, sizeof(Sym));
  s->name = pc_strdup(d->name);
  PcType *tc = pc_type_clone(t);
  pc_value_init_array(&s->val, tc, cells);
  sym_put(I->env, s);
}

static void exec_stmt(Interp *I, PcAst *s) {
  if (!s) return;
  if (I->trace) {
    fprintf(stderr, "[trace] ");
    if (s->loc.filename) fprintf(stderr, "%s:%d: ", s->loc.filename, s->loc.line);
    fprintf(stderr, "stmt\n");
  }
  switch (s->kind) {
  case PC_AST_DECLARE: {
    if (sym_find_local(I->env, s->name)) {
      pc_runtime_error(I->err, s->loc, "redeclaration of '%s'", s->name);
      break;
    }
    Sym *sy = calloc(1, sizeof(Sym));
    sy->name = pc_strdup(s->name);
    memset(&sy->val, 0, sizeof(sy->val));
    sy->val.type = pc_type_clone(s->vtype);
    if (!sy->val.type) break;
    switch (sy->val.type->kind) {
    case PC_T_INT:
      sy->val.i = 0;
      break;
    case PC_T_REAL:
      sy->val.r = 0.0;
      break;
    case PC_T_BOOL:
      sy->val.b = false;
      break;
    case PC_T_CHAR:
      sy->val.c = ' ';
      break;
    case PC_T_STRING:
      sy->val.s = pc_strdup("");
      sy->val.owned_str = true;
      break;
    case PC_T_DATE:
      sy->val.s = pc_strdup("01/01/2000");
      sy->val.owned_str = true;
      break;
    default:
      break;
    }
    sym_put(I->env, sy);
    break;
  }
  case PC_AST_DECLARE_ARRAY:
    if (sym_find_local(I->env, s->name)) {
      pc_runtime_error(I->err, s->loc, "redeclaration of '%s'", s->name);
      break;
    }
    declare_array(I, s);
    break;
  case PC_AST_CONSTANT: {
    if (sym_find_local(I->env, s->name)) {
      pc_runtime_error(I->err, s->loc, "redeclaration of '%s'", s->name);
      break;
    }
    Sym *sy = calloc(1, sizeof(Sym));
    sy->name = pc_strdup(s->name);
    sy->is_const = true;
    eval_expr(I, s->init_expr, &sy->val);
    sym_put(I->env, sy);
    break;
  }
  case PC_AST_ASSIGN: {
    PcValue rhs;
    eval_expr(I, s->right, &rhs);
    if (s->left->kind == PC_AST_VAR) {
      Sym *ls = sym_find(I, s->left->name);
      if (ls && rhs.type && rhs.type->kind == PC_T_ARRAY && ls->val.type && ls->val.type->kind == PC_T_ARRAY) {
        pc_value_destroy(&ls->val);
        pc_value_copy(&ls->val, &rhs);
        pc_value_destroy(&rhs);
        break;
      }
    }
    PcValue *p = lvalue_ptr(I, s->left, s->loc);
    if (p) {
      pc_value_destroy(p);
      pc_value_copy(p, &rhs);
    }
    pc_value_destroy(&rhs);
    break;
  }
  case PC_AST_IF: {
    PcValue c;
    eval_expr(I, s->cond, &c);
    bool t = pc_value_truthy(&c);
    pc_value_destroy(&c);
    if (t)
      exec_block(I, s->then_arm);
    else
      exec_block(I, s->else_arm);
    break;
  }
  case PC_AST_CASE: {
    Sym *sv = sym_find(I, s->case_var);
    if (!sv) {
      pc_runtime_error(I->err, s->loc, "CASE variable '%s' is not defined", s->case_var);
      break;
    }
    PcValue disc;
    pc_value_copy(&disc, &(sv->alias_of ? sv->alias_of : sv)->val);
    for (PcAst *arm = s->cases; arm; arm = arm->cases) {
      if (arm->is_otherwise) {
        exec_block(I, arm->body);
        break;
      }
      PcValue lo, hi;
      eval_expr(I, arm->range_lo, &lo);
      eval_expr(I, arm->range_hi, &hi);
      bool hit = false;
      if (disc.type && disc.type->kind == PC_T_INT) {
        int64_t d = disc.i, a = to_int_compat(&lo), b = to_int_compat(&hi);
        hit = d >= a && d <= b;
      } else if (disc.type && disc.type->kind == PC_T_CHAR) {
        char a = (lo.type && lo.type->kind == PC_T_CHAR) ? lo.c : (char)to_int_compat(&lo);
        char b = (hi.type && hi.type->kind == PC_T_CHAR) ? hi.c : (char)to_int_compat(&hi);
        hit = disc.c >= a && disc.c <= b;
      } else if (disc.type && (disc.type->kind == PC_T_STRING || disc.type->kind == PC_T_DATE)) {
        hit = strcmp(disc.s ? disc.s : "", lo.s ? lo.s : "") == 0;
      }
      pc_value_destroy(&lo);
      pc_value_destroy(&hi);
      if (hit) {
        exec_block(I, arm->body);
        break;
      }
    }
    pc_value_destroy(&disc);
    break;
  }
  case PC_AST_FOR: {
    int a = eval_int(I, s->for_start);
    int b = eval_int(I, s->for_stop);
    int step = s->for_step ? eval_int(I, s->for_step) : 1;
    Sym *lv = sym_find(I, s->iter);
    if (!lv) {
      pc_runtime_error(I->err, s->loc, "FOR loop variable not declared");
      break;
    }
    PcValue *slot = &(lv->alias_of ? lv->alias_of : lv)->val;
    if (step == 0) {
      pc_runtime_error(I->err, s->loc, "FOR STEP cannot be zero");
      break;
    }
    if (step > 0) {
      for (int x = a; x <= b; x += step) {
        pc_value_destroy(slot);
        pc_value_init_int(slot, x);
        exec_block(I, s->body);
      }
    } else {
      for (int x = a; x >= b; x += step) {
        pc_value_destroy(slot);
        pc_value_init_int(slot, x);
        exec_block(I, s->body);
      }
    }
    break;
  }
  case PC_AST_WHILE: {
    for (;;) {
      PcValue c;
      eval_expr(I, s->cond, &c);
      bool t = pc_value_truthy(&c);
      pc_value_destroy(&c);
      if (!t) break;
      exec_block(I, s->body);
    }
    break;
  }
  case PC_AST_REPEAT:
    do {
      exec_block(I, s->body);
      PcValue c;
      eval_expr(I, s->cond, &c);
      bool t = pc_value_truthy(&c);
      pc_value_destroy(&c);
      if (t) break;
    } while (1);
    break;
  case PC_AST_CALL: {
    NamedAst *p = named_find(I->procs, s->name);
    if (!p) {
      pc_runtime_error(I->err, s->loc, "undefined procedure '%s'", s->name);
      break;
    }
    PcAst *pd = p->def;
    Env *local = env_new(I->global);
    for (size_t i = 0; i < pd->param_count; i++) {
      Sym *sym = calloc(1, sizeof(Sym));
      sym->name = pc_strdup(pd->params[i].name);
      if (i < s->arg_count) {
        if (pd->params[i].by_ref) {
          PcValue *ptr = lvalue_ptr(I, s->args[i], s->loc);
          if (!ptr) {
            pc_runtime_error(I->err, s->loc, "BYREF argument must be a variable");
            free(sym->name);
            free(sym);
            continue;
          }
          Sym *target = sym_find(I, s->args[i]->name);
          if (s->args[i]->kind == PC_AST_VAR && target) {
            sym->alias_of = target->alias_of ? target->alias_of : target;
          } else {
            pc_runtime_error(I->err, s->loc, "BYREF requires a simple variable");
          }
        } else {
          PcValue v;
          eval_expr(I, s->args[i], &v);
          pc_value_copy(&sym->val, &v);
          pc_value_destroy(&v);
        }
      }
      sym_put(local, sym);
    }
    Env *saved = I->env;
    I->env = local;
    for (PcAst *st = pd->body; st; st = st->next) exec_stmt(I, st);
    I->env = saved;
    env_free(local);
    break;
  }
  case PC_AST_RETURN:
    eval_expr(I, s->init_expr, &I->ret);
    I->returned = true;
    break;
  case PC_AST_INPUT: {
    PcValue *p = lvalue_ptr(I, s->init_expr, s->loc);
    if (!p || !p->type) break;
    PcTypeKind tk = p->type->kind;
    char buf[4096];
    if (!fgets(buf, sizeof buf, stdin)) buf[0] = '\0';
    size_t n = strlen(buf);
    if (n && buf[n - 1] == '\n') buf[n - 1] = '\0';
    pc_value_destroy(p);
    if (tk == PC_T_INT)
      pc_value_init_int(p, (int64_t)strtoll(buf, NULL, 10));
    else if (tk == PC_T_REAL)
      pc_value_init_real(p, strtod(buf, NULL));
    else if (tk == PC_T_BOOL)
      pc_value_init_bool(p, buf[0] == 'T' || buf[0] == 't' || buf[0] == '1');
    else if (tk == PC_T_CHAR)
      pc_value_init_char(p, buf[0] ? buf[0] : ' ');
    else if (tk == PC_T_DATE) {
      pc_value_init_string(p, pc_strdup(buf), true);
      if (p->type) p->type->kind = PC_T_DATE;
    } else
      pc_value_init_string(p, pc_strdup(buf), true);
    break;
  }
  case PC_AST_OUTPUT:
    for (size_t i = 0; i < s->arg_count; i++) {
      PcValue v;
      eval_expr(I, s->args[i], &v);
      if (v.type && (v.type->kind == PC_T_STRING || v.type->kind == PC_T_DATE))
        printf("%s", v.s ? v.s : "");
      else if (v.type && v.type->kind == PC_T_INT)
        printf("%lld", (long long)v.i);
      else if (v.type && v.type->kind == PC_T_REAL)
        printf("%g", v.r);
      else if (v.type && v.type->kind == PC_T_BOOL)
        printf("%s", v.b ? "TRUE" : "FALSE");
      else if (v.type && v.type->kind == PC_T_CHAR)
        printf("%c", v.c);
      pc_value_destroy(&v);
      if (i + 1 < s->arg_count) printf(" ");
    }
    printf("\n");
    break;
  case PC_AST_OPENFILE: {
    PcValue pathv;
    eval_expr(I, s->file_expr, &pathv);
    const char *path = pathv.s ? pathv.s : "";
    const char *mode = s->open_mode ? s->open_mode : "READ";
    FILE *fp = NULL;
    if (pc_str_eq_ci(mode, "READ"))
      fp = fopen(path, "r");
    else if (pc_str_eq_ci(mode, "WRITE"))
      fp = fopen(path, "w");
    else if (pc_str_eq_ci(mode, "APPEND"))
      fp = fopen(path, "a");
    else if (pc_str_eq_ci(mode, "RANDOM"))
      fp = fopen(path, "r+b");
    if (!fp && pc_str_eq_ci(mode, "RANDOM")) fp = fopen(path, "w+b");
    if (!fp) {
      pc_runtime_error(I->err, s->loc, "could not open file '%s'", path);
    } else {
      int m = 0;
      if (pc_str_eq_ci(mode, "WRITE")) m = 1;
      else if (pc_str_eq_ci(mode, "APPEND")) m = 2;
      else if (pc_str_eq_ci(mode, "RANDOM")) m = 3;
      file_set(I, path, fp, m);
    }
    pc_value_destroy(&pathv);
    break;
  }
  case PC_AST_CLOSEFILE: {
    PcValue pathv;
    eval_expr(I, s->file_expr, &pathv);
    const char *path = pathv.s ? pathv.s : "";
    for (size_t i = 0; i < I->file_count; i++) {
      if (I->files[i].path && strcmp(I->files[i].path, path) == 0) {
        if (I->files[i].fp) fclose(I->files[i].fp);
        I->files[i].fp = NULL;
        break;
      }
    }
    pc_value_destroy(&pathv);
    break;
  }
  case PC_AST_READFILE: {
    PcValue pathv;
    eval_expr(I, s->file_expr, &pathv);
    const char *path = pathv.s ? pathv.s : "";
    FILE *fp = find_file_fp(I, path);
    PcValue *p = lvalue_ptr(I, s->init_expr, s->loc);
    char buf[8192];
    if (fp && fgets(buf, sizeof buf, fp)) {
      size_t n = strlen(buf);
      if (n && buf[n - 1] == '\n') buf[n - 1] = '\0';
      if (p) {
        pc_value_destroy(p);
        pc_value_init_string(p, pc_strdup(buf), true);
      }
    } else if (p) {
      pc_value_destroy(p);
      pc_value_init_string(p, pc_strdup(""), true);
    }
    pc_value_destroy(&pathv);
    break;
  }
  case PC_AST_WRITEFILE: {
    PcValue pathv, data;
    eval_expr(I, s->file_expr, &pathv);
    eval_expr(I, s->init_expr, &data);
    const char *path = pathv.s ? pathv.s : "";
    FILE *fp = find_file_fp(I, path);
    if (fp && data.type) {
      if (data.type->kind == PC_T_STRING || data.type->kind == PC_T_DATE)
        fputs(data.s ? data.s : "", fp);
      else if (data.type->kind == PC_T_INT)
        fprintf(fp, "%lld", (long long)data.i);
      else if (data.type->kind == PC_T_REAL)
        fprintf(fp, "%g", data.r);
      fputc('\n', fp);
    }
    pc_value_destroy(&pathv);
    pc_value_destroy(&data);
    break;
  }
  case PC_AST_PROC:
  case PC_AST_FUNC:
    break;
  default:
    break;
  }
}

static void exec_block(Interp *I, PcAst *s) {
  for (; s; s = s->next) {
    exec_stmt(I, s);
    if (I->returned) return;
  }
}

bool pc_interp_run(Interp *I, PcAst *program) {
  if (!program || program->kind != PC_AST_PROGRAM) return false;
  register_defs(I, program->stmts);
  srand((unsigned)time(NULL));
  exec_block(I, program->stmts);
  return !I->err->had_error;
}

bool pc_interp_check(Interp *I, PcAst *program) {
  (void)I;
  return program && program->kind == PC_AST_PROGRAM;
}
