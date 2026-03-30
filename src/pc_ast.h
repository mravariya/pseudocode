#ifndef PC_AST_H
#define PC_AST_H

#include "pc_token.h"
#include "pc_error.h"
#include "pc_value.h"
#include <stddef.h>

typedef struct PcAst PcAst;

typedef enum PcAstKind {
  PC_AST_PROGRAM,
  PC_AST_BLOCK,
  PC_AST_DECLARE,
  PC_AST_DECLARE_ARRAY,
  PC_AST_CONSTANT,
  PC_AST_ASSIGN,
  PC_AST_IF,
  PC_AST_CASE,
  PC_AST_CASE_ARM,
  PC_AST_FOR,
  PC_AST_WHILE,
  PC_AST_REPEAT,
  PC_AST_PROC,
  PC_AST_FUNC,
  PC_AST_CALL,
  PC_AST_RETURN,
  PC_AST_INPUT,
  PC_AST_OUTPUT,
  PC_AST_OPENFILE,
  PC_AST_CLOSEFILE,
  PC_AST_READFILE,
  PC_AST_WRITEFILE,
  PC_AST_EXPR_STMT,

  PC_AST_BINARY,
  PC_AST_UNARY,
  PC_AST_LITERAL,
  PC_AST_VAR,
  PC_AST_INDEX,
  PC_AST_CALL_EXPR,
} PcAstKind;

typedef struct PcParam {
  char *name;
  PcType *type;
  bool by_ref;
} PcParam;

struct PcAst {
  PcAstKind kind;
  PcSourceLoc loc;
  PcAst *next;

  char *name;
  PcType *vtype;
  PcAst *init_expr;

  PcAst *cond;
  PcAst *then_arm;
  PcAst *else_arm;
  PcAst *body;
  PcAst *cases;
  char *case_var;

  PcAst *for_start;
  PcAst *for_stop;
  PcAst *for_step;
  char *iter;

  PcParam *params;
  size_t param_count;
  PcAst *stmts;

  PcTokenKind op;
  PcAst *left;
  PcAst *right;
  PcAst *child;

  int64_t int_val;
  double real_val;
  bool bool_val;
  char char_val;
  char *str_val;
  bool lit_is_real;
  bool lit_is_char;
  bool lit_is_bool;
  int date_day, date_mon, date_year;

  PcAst **args;
  size_t arg_count;

  char *open_mode;
  PcAst *file_expr;

  PcAst *range_lo;
  PcAst *range_hi;
  bool is_otherwise;
};

PcAst *pc_ast_new(PcAstKind k, PcSourceLoc loc);
void pc_ast_free(PcAst *a);

#endif
