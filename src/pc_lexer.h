#ifndef PC_LEXER_H
#define PC_LEXER_H

#include "pc_token.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct PcLexer {
  const char *filename;
  const char *src;
  size_t len;
  size_t pos;
  int line;
  int column;
  PcToken cur;
  char *lexeme_buf;
  size_t lexeme_cap;
} PcLexer;

void pc_lex_init(PcLexer *L, const char *filename, const char *src, size_t len);
void pc_lex_free(PcLexer *L);
void pc_lex_next(PcLexer *L);
bool pc_lex_match(PcLexer *L, PcTokenKind k);
void pc_lex_skip_newlines(PcLexer *L);

typedef struct PcLexState {
  size_t pos;
  int line;
  int column;
  PcToken cur;
} PcLexState;

void pc_lex_save(const PcLexer *L, PcLexState *s);
void pc_lex_restore(PcLexer *L, const PcLexState *s);

#endif
