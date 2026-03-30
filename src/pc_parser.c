#include "pc_parser.h"
#include "pc_common.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  PcLexer *L;
  PcErrorCtx *err;
} Parser;

#define LOC_FROM(P)                                                                              \
  ((PcSourceLoc){((P)->L->filename), ((P)->L->cur.line), ((P)->L->cur.column)})

static char *lex_dup(PcLexer *L) {
  return pc_strndup(L->cur.lexeme, L->cur.lexeme_len);
}

static void expect_tok(Parser *P, PcTokenKind k, const char *what) {
  if (P->L->cur.kind != k) {
    pc_error_at(P->err, LOC_FROM(P), "expected %s, got token", what);
  } else
    pc_lex_next(P->L);
}

static PcType *parse_simple_type(Parser *P) {
  PcLexer *L = P->L;
  PcType *t = calloc(1, sizeof(PcType));
  if (!t) return NULL;
  switch (L->cur.kind) {
  case TOK_INTEGER:
    t->kind = PC_T_INT;
    pc_lex_next(L);
    break;
  case TOK_REAL_T:
    t->kind = PC_T_REAL;
    pc_lex_next(L);
    break;
  case TOK_STRING_T:
    t->kind = PC_T_STRING;
    pc_lex_next(L);
    break;
  case TOK_CHAR_T:
    t->kind = PC_T_CHAR;
    pc_lex_next(L);
    break;
  case TOK_BOOLEAN:
    t->kind = PC_T_BOOL;
    pc_lex_next(L);
    break;
  case TOK_DATE_T:
    t->kind = PC_T_DATE;
    pc_lex_next(L);
    break;
  default:
    free(t);
    pc_error_at(P->err, LOC_FROM(P), "expected type name");
    return NULL;
  }
  return t;
}

static PcAst *parse_expr(Parser *P);

/* Assignment-style RHS: expr (',' expr)* — comma joins strings at runtime (Extension). */
static PcAst *parse_expr_assign_rhs(Parser *P) {
  PcAst *x = parse_expr(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_COMMA) {
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = TOK_COMMA;
    b->left = x;
    b->right = parse_expr(P);
    x = b;
  }
  return x;
}

/* forward */
static PcAst *parse_stmt(Parser *P);
static PcAst *parse_lvalue(Parser *P);

static PcAst *parse_primary(Parser *P) {
  PcLexer *L = P->L;
  PcSourceLoc loc = LOC_FROM(P);
  if (L->cur.kind == TOK_INT) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->int_val = (int64_t)strtoll(L->cur.lexeme, NULL, 10);
    a->lit_is_real = false;
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_REAL) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->real_val = strtod(L->cur.lexeme, NULL);
    a->lit_is_real = true;
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_STRING) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->str_val = lex_dup(L);
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_CHAR) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->char_val = L->cur.lexeme && L->cur.lexeme_len ? L->cur.lexeme[0] : '\0';
    a->lit_is_char = true;
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_DATE) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->str_val = pc_strndup(L->cur.lexeme, L->cur.lexeme_len);
    /* mark as date via type in interp from literal context */
    a->date_day = 0;
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_TRUE || L->cur.kind == TOK_FALSE) {
    PcAst *a = pc_ast_new(PC_AST_LITERAL, loc);
    a->bool_val = L->cur.kind == TOK_TRUE;
    a->lit_is_bool = true;
    pc_lex_next(L);
    return a;
  }
  if (L->cur.kind == TOK_LPAREN) {
    pc_lex_next(L);
    PcAst *e = parse_expr(P);
    expect_tok(P, TOK_RPAREN, ")");
    return e;
  }
  if (L->cur.kind == TOK_IDENT) {
    PcAst *v = pc_ast_new(PC_AST_VAR, loc);
    v->name = lex_dup(L);
    pc_lex_next(L);
    if (L->cur.kind == TOK_LBRACK) {
      pc_lex_next(L);
      PcAst *i1 = parse_expr(P);
      PcAst *ix = pc_ast_new(PC_AST_INDEX, loc);
      ix->left = v;
      ix->child = i1;
      if (L->cur.kind == TOK_COMMA) {
        pc_lex_next(L);
        PcAst *i2 = parse_expr(P);
        PcAst *ix2 = pc_ast_new(PC_AST_INDEX, loc);
        ix2->left = ix;
        ix2->child = i2;
        expect_tok(P, TOK_RBRACK, "]");
        return ix2;
      }
      expect_tok(P, TOK_RBRACK, "]");
      return ix;
    }
    if (L->cur.kind == TOK_LPAREN) {
      pc_lex_next(L);
      PcAst *c = pc_ast_new(PC_AST_CALL_EXPR, loc);
      c->name = v->name;
      v->name = NULL;
      pc_ast_free(v);
      size_t cap = 0;
      if (L->cur.kind != TOK_RPAREN) {
        while (1) {
          PC_ARRAY_GROW(c->args, cap, c->arg_count + 1, sizeof(PcAst *));
          c->args[c->arg_count++] = parse_expr(P);
          if (L->cur.kind == TOK_COMMA)
            pc_lex_next(L);
          else
            break;
        }
      }
      expect_tok(P, TOK_RPAREN, ")");
      return c;
    }
    return v;
  }
  pc_error_at(P->err, loc, "expected expression");
  return NULL;
}

static PcAst *parse_unary(Parser *P) {
  PcLexer *L = P->L;
  if (L->cur.kind == TOK_NOT || L->cur.kind == TOK_MINUS) {
    PcTokenKind op = L->cur.kind;
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *u = pc_ast_new(PC_AST_UNARY, loc);
    u->op = op;
    u->child = parse_unary(P);
    return u;
  }
  return parse_primary(P);
}

static PcAst *parse_mul(Parser *P) {
  PcAst *x = parse_unary(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_STAR || L->cur.kind == TOK_SLASH || L->cur.kind == TOK_DIV ||
         L->cur.kind == TOK_MOD) {
    PcTokenKind op = L->cur.kind;
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = op;
    b->left = x;
    b->right = parse_unary(P);
    x = b;
  }
  return x;
}

static PcAst *parse_add(Parser *P) {
  PcAst *x = parse_mul(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_PLUS || L->cur.kind == TOK_MINUS) {
    PcTokenKind op = L->cur.kind;
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = op;
    b->left = x;
    b->right = parse_mul(P);
    x = b;
  }
  return x;
}

static PcAst *parse_concat(Parser *P) {
  PcAst *x = parse_add(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_AMP) {
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = TOK_AMP;
    b->left = x;
    b->right = parse_add(P);
    x = b;
  }
  return x;
}

static PcAst *parse_rel(Parser *P) {
  PcAst *x = parse_concat(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_EQ || L->cur.kind == TOK_NE || L->cur.kind == TOK_LT ||
         L->cur.kind == TOK_GT || L->cur.kind == TOK_LE || L->cur.kind == TOK_GE) {
    PcTokenKind op = L->cur.kind;
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = op;
    b->left = x;
    b->right = parse_concat(P);
    x = b;
  }
  return x;
}

static PcAst *parse_and(Parser *P) {
  PcAst *x = parse_rel(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_AND) {
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = TOK_AND;
    b->left = x;
    b->right = parse_rel(P);
    x = b;
  }
  return x;
}

static PcAst *parse_expr(Parser *P) {
  PcAst *x = parse_and(P);
  PcLexer *L = P->L;
  while (L->cur.kind == TOK_OR) {
    PcSourceLoc loc = LOC_FROM(P);
    pc_lex_next(L);
    PcAst *b = pc_ast_new(PC_AST_BINARY, loc);
    b->op = TOK_OR;
    b->left = x;
    b->right = parse_and(P);
    x = b;
  }
  return x;
}

static PcAst *parse_lvalue(Parser *P) { return parse_primary(P); }

static PcAst *parse_stmt_list_until(Parser *P, PcTokenKind *stops, size_t nstop) {
  PcLexer *L = P->L;
  PcAst *head = NULL, *tail = NULL;
  for (;;) {
    pc_lex_skip_newlines(L);
    if (L->cur.kind == TOK_EOF) break;
    for (size_t i = 0; i < nstop; i++) {
      if (L->cur.kind == stops[i]) return head;
    }
    PcAst *s = parse_stmt(P);
    if (!s) break;
    if (!head)
      head = tail = s;
    else {
      tail->next = s;
      tail = s;
    }
  }
  return head;
}

static PcAst *parse_block_until(Parser *P, PcTokenKind *stops, size_t n) {
  return parse_stmt_list_until(P, stops, n);
}

static void parse_param_list(Parser *P, PcAst *fn) {
  PcLexer *L = P->L;
  if (L->cur.kind == TOK_RPAREN) return;
  size_t cap = 0;
  for (;;) {
    bool byref = false, byval = false;
    if (L->cur.kind == TOK_BYREF) {
      byref = true;
      pc_lex_next(L);
    } else if (L->cur.kind == TOK_BYVAL) {
      byval = true;
      pc_lex_next(L);
    }
    (void)byval;
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected parameter name");
      return;
    }
    PC_ARRAY_GROW(fn->params, cap, fn->param_count + 1, sizeof(PcParam));
    PcParam *p = &fn->params[fn->param_count++];
    p->name = lex_dup(L);
    p->by_ref = byref;
    pc_lex_next(L);
    expect_tok(P, TOK_COLON, ":");
    p->type = parse_simple_type(P);
    if (L->cur.kind == TOK_COMMA)
      pc_lex_next(L);
    else
      break;
  }
}

static PcAst *parse_stmt(Parser *P) {
  PcLexer *L = P->L;
  pc_lex_skip_newlines(L);
  PcSourceLoc loc = LOC_FROM(P);

  if (L->cur.kind == TOK_DECLARE) {
    pc_lex_next(L);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected identifier after DECLARE");
      return NULL;
    }
    char *id = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_COLON, ":");
    if (L->cur.kind == TOK_ARRAY) {
      pc_lex_next(L);
      expect_tok(P, TOK_LBRACK, "[");
      PcAst *lo = parse_expr(P);
      expect_tok(P, TOK_COLON, ":");
      PcAst *hi = parse_expr(P);
      PcAst *lo2 = NULL, *hi2 = NULL;
      bool d2 = false;
      if (L->cur.kind == TOK_COMMA) {
        pc_lex_next(L);
        lo2 = parse_expr(P);
        expect_tok(P, TOK_COLON, ":");
        hi2 = parse_expr(P);
        d2 = true;
      }
      expect_tok(P, TOK_RBRACK, "]");
      expect_tok(P, TOK_OF, "OF");
      PcType *el = parse_simple_type(P);
      PcAst *d = pc_ast_new(PC_AST_DECLARE_ARRAY, loc);
      d->name = id;
      d->vtype = calloc(1, sizeof(PcType));
      if (d->vtype) {
        d->vtype->kind = PC_T_ARRAY;
        d->vtype->elem = el;
        d->vtype->is2d = d2;
      }
      d->for_start = lo;
      d->for_stop = hi;
      d->for_step = lo2;
      d->child = hi2;
      return d;
    }
    PcType *t = parse_simple_type(P);
    PcAst *d = pc_ast_new(PC_AST_DECLARE, loc);
    d->name = id;
    d->vtype = t;
    return d;
  }

  if (L->cur.kind == TOK_CONSTANT) {
    pc_lex_next(L);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected identifier after CONSTANT");
      return NULL;
    }
    PcAst *c = pc_ast_new(PC_AST_CONSTANT, loc);
    c->name = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_EQ, "=");
    c->init_expr = parse_expr_assign_rhs(P);
    return c;
  }

  if (L->cur.kind == TOK_IF) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_IF, loc);
    n->cond = parse_expr(P);
    expect_tok(P, TOK_THEN, "THEN");
    PcTokenKind s1[] = {TOK_ELSE, TOK_ENDIF};
    n->then_arm = parse_block_until(P, s1, 2);
    if (L->cur.kind == TOK_ELSE) {
      pc_lex_next(L);
      PcTokenKind s2[] = {TOK_ENDIF};
      n->else_arm = parse_block_until(P, s2, 1);
    }
    expect_tok(P, TOK_ENDIF, "ENDIF");
    return n;
  }

  if (L->cur.kind == TOK_CASE) {
    pc_lex_next(L);
    expect_tok(P, TOK_OF, "OF");
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected identifier after CASE OF");
      return NULL;
    }
    PcAst *n = pc_ast_new(PC_AST_CASE, loc);
    n->case_var = lex_dup(L);
    pc_lex_next(L);
    PcAst *ahead = NULL, *atail = NULL;
    bool done = false;
    while (!done) {
      pc_lex_skip_newlines(L);
      if (L->cur.kind == TOK_ENDCASE) {
        done = true;
        break;
      }
      PcAst *arm = pc_ast_new(PC_AST_CASE_ARM, LOC_FROM(P));
      if (L->cur.kind == TOK_OTHERWISE) {
        pc_lex_next(L);
        expect_tok(P, TOK_COLON, ":");
        arm->is_otherwise = true;
        PcAst *bh = NULL, *bt = NULL;
        for (;;) {
          pc_lex_skip_newlines(L);
          if (L->cur.kind == TOK_ENDCASE) {
            done = true;
            break;
          }
          PcAst *st = parse_stmt(P);
          if (!st) break;
          if (!bh)
            bh = bt = st;
          else {
            bt->next = st;
            bt = st;
          }
        }
        arm->body = bh;
        if (!ahead)
          ahead = atail = arm;
        else {
          atail->cases = arm;
          atail = arm;
        }
        break;
      }
      arm->range_lo = parse_expr(P);
      arm->range_hi = arm->range_lo;
      if (L->cur.kind == TOK_TO) {
        pc_lex_next(L);
        arm->range_hi = parse_expr(P);
      }
      expect_tok(P, TOK_COLON, ":");
      PcAst *bh = NULL, *bt = NULL;
      for (;;) {
        pc_lex_skip_newlines(L);
        if (L->cur.kind == TOK_ENDCASE) {
          done = true;
          break;
        }
        if (L->cur.kind == TOK_OTHERWISE) break;
        PcLexState sv;
        pc_lex_save(L, &sv);
        PcAst *probe = parse_expr(P);
        if (probe && L->cur.kind == TOK_COLON) {
          pc_ast_free(probe);
          pc_lex_restore(L, &sv);
          break;
        }
        if (probe) pc_ast_free(probe);
        pc_lex_restore(L, &sv);
        PcAst *st = parse_stmt(P);
        if (!st) break;
        if (!bh)
          bh = bt = st;
        else {
          bt->next = st;
          bt = st;
        }
      }
      arm->body = bh;
      if (!ahead)
        ahead = atail = arm;
      else {
        atail->cases = arm;
        atail = arm;
      }
    }
    expect_tok(P, TOK_ENDCASE, "ENDCASE");
    n->cases = ahead;
    return n;
  }

  if (L->cur.kind == TOK_FOR) {
    pc_lex_next(L);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected loop variable");
      return NULL;
    }
    PcAst *n = pc_ast_new(PC_AST_FOR, loc);
    n->iter = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_ASSIGN, "<-");
    n->for_start = parse_expr(P);
    expect_tok(P, TOK_TO, "TO");
    n->for_stop = parse_expr(P);
    if (L->cur.kind == TOK_STEP) {
      pc_lex_next(L);
      n->for_step = parse_expr(P);
    }
    PcTokenKind st[] = {TOK_NEXT};
    n->body = parse_block_until(P, st, 1);
    expect_tok(P, TOK_NEXT, "NEXT");
    if (L->cur.kind == TOK_IDENT) {
      free(n->iter);
      n->iter = lex_dup(L);
      pc_lex_next(L);
    }
    return n;
  }

  if (L->cur.kind == TOK_WHILE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_WHILE, loc);
    n->cond = parse_expr(P);
    PcTokenKind st[] = {TOK_ENDWHILE};
    n->body = parse_block_until(P, st, 1);
    expect_tok(P, TOK_ENDWHILE, "ENDWHILE");
    return n;
  }

  if (L->cur.kind == TOK_REPEAT) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_REPEAT, loc);
    PcTokenKind st[] = {TOK_UNTIL};
    n->body = parse_block_until(P, st, 1);
    expect_tok(P, TOK_UNTIL, "UNTIL");
    n->cond = parse_expr(P);
    return n;
  }

  if (L->cur.kind == TOK_PROCEDURE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_PROC, loc);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected procedure name");
      return NULL;
    }
    n->name = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_LPAREN, "(");
    parse_param_list(P, n);
    expect_tok(P, TOK_RPAREN, ")");
    PcTokenKind st[] = {TOK_ENDPROCEDURE};
    n->body = parse_block_until(P, st, 1);
    expect_tok(P, TOK_ENDPROCEDURE, "ENDPROCEDURE");
    return n;
  }

  if (L->cur.kind == TOK_FUNCTION) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_FUNC, loc);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected function name");
      return NULL;
    }
    n->name = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_LPAREN, "(");
    parse_param_list(P, n);
    expect_tok(P, TOK_RPAREN, ")");
    expect_tok(P, TOK_RETURNS, "RETURNS");
    n->vtype = parse_simple_type(P);
    PcTokenKind st[] = {TOK_ENDFUNCTION};
    n->body = parse_block_until(P, st, 1);
    expect_tok(P, TOK_ENDFUNCTION, "ENDFUNCTION");
    return n;
  }

  if (L->cur.kind == TOK_CALL) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_CALL, loc);
    if (L->cur.kind != TOK_IDENT) {
      pc_error_at(P->err, LOC_FROM(P), "expected procedure name after CALL");
      return NULL;
    }
    n->name = lex_dup(L);
    pc_lex_next(L);
    expect_tok(P, TOK_LPAREN, "(");
    size_t cap = 0;
    if (L->cur.kind != TOK_RPAREN) {
      while (1) {
        PC_ARRAY_GROW(n->args, cap, n->arg_count + 1, sizeof(PcAst *));
        n->args[n->arg_count++] = parse_expr(P);
        if (L->cur.kind == TOK_COMMA)
          pc_lex_next(L);
        else
          break;
      }
    }
    expect_tok(P, TOK_RPAREN, ")");
    return n;
  }

  if (L->cur.kind == TOK_RETURN) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_RETURN, loc);
    n->init_expr = parse_expr_assign_rhs(P);
    return n;
  }

  if (L->cur.kind == TOK_INPUT) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_INPUT, loc);
    n->init_expr = parse_lvalue(P);
    return n;
  }

  if (L->cur.kind == TOK_OUTPUT) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_OUTPUT, loc);
    size_t cap = 0;
    while (L->cur.kind != TOK_NEWLINE && L->cur.kind != TOK_EOF) {
      PC_ARRAY_GROW(n->args, cap, n->arg_count + 1, sizeof(PcAst *));
      n->args[n->arg_count++] = parse_expr(P);
      if (L->cur.kind == TOK_COMMA)
        pc_lex_next(L);
      else
        break;
    }
    return n;
  }

  if (L->cur.kind == TOK_OPENFILE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_OPENFILE, loc);
    n->file_expr = parse_expr(P);
    expect_tok(P, TOK_FOR, "FOR");
    if (L->cur.kind == TOK_IDENT) {
      char *m = lex_dup(L);
      pc_lex_next(L);
      if (pc_str_eq_ci(m, "read") || pc_str_eq_ci(m, "write") || pc_str_eq_ci(m, "append") ||
          pc_str_eq_ci(m, "random")) {
        n->open_mode = m;
      } else {
        free(m);
        pc_error_at(P->err, LOC_FROM(P), "expected READ, WRITE, APPEND, or RANDOM after FOR");
      }
    } else {
      pc_error_at(P->err, LOC_FROM(P), "expected READ, WRITE, APPEND, or RANDOM after FOR");
    }
    return n;
  }

  if (L->cur.kind == TOK_CLOSEFILE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_CLOSEFILE, loc);
    n->file_expr = parse_expr(P);
    return n;
  }

  if (L->cur.kind == TOK_READFILE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_READFILE, loc);
    n->file_expr = parse_expr(P);
    expect_tok(P, TOK_COMMA, ",");
    n->init_expr = parse_lvalue(P);
    return n;
  }

  if (L->cur.kind == TOK_WRITEFILE) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_WRITEFILE, loc);
    n->file_expr = parse_expr(P);
    expect_tok(P, TOK_COMMA, ",");
    n->init_expr = parse_expr(P);
    return n;
  }

  /* assignment */
  PcLexState sv;
  pc_lex_save(L, &sv);
  PcAst *lhs = parse_lvalue(P);
  if (lhs && L->cur.kind == TOK_ASSIGN) {
    pc_lex_next(L);
    PcAst *n = pc_ast_new(PC_AST_ASSIGN, loc);
    n->left = lhs;
    n->right = parse_expr_assign_rhs(P);
    return n;
  }
  if (lhs) pc_ast_free(lhs);
  pc_lex_restore(L, &sv);

  pc_error_at(P->err, LOC_FROM(P), "unexpected token at start of statement");
  return NULL;
}

PcAst *pc_parse_program(PcLexer *L, PcErrorCtx *err) {
  Parser P = {L, err};
  PcAst *prog = pc_ast_new(PC_AST_PROGRAM, (PcSourceLoc){L->filename, 1, 1});
  PcTokenKind st[] = {TOK_EOF};
  prog->stmts = parse_stmt_list_until(&P, st, 1);
  pc_lex_skip_newlines(L);
  if (L->cur.kind != TOK_EOF)
    pc_error_at(err, (PcSourceLoc){L->filename, L->cur.line, L->cur.column}, "expected end of file");
  return prog;
}
