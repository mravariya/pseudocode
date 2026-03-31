#ifndef PC_TOKEN_H
#define PC_TOKEN_H

#include <stddef.h>

typedef enum PcTokenKind {
  TOK_EOF = 0,
  TOK_ERROR,
  TOK_IDENT,
  TOK_INT,
  TOK_REAL,
  TOK_STRING,
  TOK_CHAR,
  TOK_DATE,
  TOK_TRUE,
  TOK_FALSE,

  TOK_DECLARE,
  TOK_CONSTANT,
  TOK_INTEGER,
  TOK_REAL_T,
  TOK_STRING_T,
  TOK_CHAR_T,
  TOK_BOOLEAN,
  TOK_DATE_T,
  TOK_ARRAY,
  TOK_OF,
  TOK_TYPE,
  TOK_ENDTYPE,

  TOK_IF,
  TOK_THEN,
  TOK_ELSE,
  TOK_ENDIF,
  TOK_CASE,
  TOK_ENDCASE,
  TOK_OTHERWISE,

  TOK_FOR,
  TOK_TO,
  TOK_STEP,
  TOK_NEXT,
  TOK_REPEAT,
  TOK_UNTIL,
  TOK_WHILE,
  TOK_ENDWHILE,

  TOK_PROCEDURE,
  TOK_ENDPROCEDURE,
  TOK_FUNCTION,
  TOK_ENDFUNCTION,
  TOK_RETURNS,
  TOK_CALL,
  TOK_RETURN,

  TOK_BYVAL,
  TOK_BYREF,

  TOK_INPUT,
  TOK_OUTPUT,

  TOK_OPENFILE,
  TOK_CLOSEFILE,
  TOK_READFILE,
  TOK_WRITEFILE,
  TOK_SEEK,
  TOK_GETRECORD,
  TOK_PUTRECORD,

  TOK_IMPORT,
  TOK_AS,

  TOK_AND,
  TOK_OR,
  TOK_NOT,
  TOK_MOD,
  TOK_DIV,

  TOK_ASSIGN,   /* ← or <- */
  TOK_EQ,
  TOK_NE,
  TOK_LE,
  TOK_GE,
  TOK_LT,
  TOK_GT,

  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACK,
  TOK_RBRACK,
  TOK_COMMA,
  TOK_COLON,
  TOK_DOT,
  TOK_AMP, /* & concat */

  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,

  TOK_NEWLINE,
} PcTokenKind;

typedef struct PcToken {
  PcTokenKind kind;
  const char *lexeme;
  size_t lexeme_len;
  int line;
  int column;
} PcToken;

const char *pc_token_name(PcTokenKind k);

#endif
