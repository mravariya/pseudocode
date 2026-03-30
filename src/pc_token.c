#include "pc_token.h"

const char *pc_token_name(PcTokenKind k) {
  switch (k) {
  case TOK_EOF:
    return "EOF";
  case TOK_ERROR:
    return "ERROR";
  case TOK_IDENT:
    return "IDENT";
  case TOK_INT:
    return "INT";
  case TOK_REAL:
    return "REAL";
  case TOK_STRING:
    return "STRING";
  case TOK_CHAR:
    return "CHAR";
  case TOK_DATE:
    return "DATE";
  case TOK_TRUE:
    return "TRUE";
  case TOK_FALSE:
    return "FALSE";
  case TOK_ASSIGN:
    return "ASSIGN";
  default:
    return "TOKEN";
  }
}
