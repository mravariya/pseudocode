#include "pc_ast.h"
#include "pc_common.h"
#include <stdlib.h>

PcAst *pc_ast_new(PcAstKind k, PcSourceLoc loc) {
  PcAst *a = calloc(1, sizeof(PcAst));
  if (!a) return NULL;
  a->kind = k;
  a->loc = loc;
  return a;
}

static void free_params(PcParam *p, size_t n) {
  for (size_t i = 0; i < n; i++) {
    free(p[i].name);
    pc_type_free(p[i].type);
  }
  free(p);
}

void pc_ast_free(PcAst *a) {
  while (a) {
    PcAst *n = a->next;
    free(a->name);
    free(a->iter);
    free(a->case_var);
    free(a->open_mode);
    free(a->str_val);
    pc_type_free(a->vtype);
    free_params(a->params, a->param_count);
    for (size_t i = 0; i < a->arg_count; i++) pc_ast_free(a->args[i]);
    free(a->args);

    pc_ast_free(a->cond);
    pc_ast_free(a->then_arm);
    pc_ast_free(a->else_arm);
    pc_ast_free(a->body);
    pc_ast_free(a->cases);
    pc_ast_free(a->init_expr);
    pc_ast_free(a->for_start);
    pc_ast_free(a->for_stop);
    pc_ast_free(a->for_step);
    pc_ast_free(a->stmts);
    pc_ast_free(a->left);
    pc_ast_free(a->right);
    pc_ast_free(a->child);
    pc_ast_free(a->file_expr);
    pc_ast_free(a->range_lo);
    pc_ast_free(a->range_hi);

    free(a);
    a = n;
  }
}
