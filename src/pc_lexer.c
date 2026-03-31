#include "pc_lexer.h"
#include "pc_common.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static void lexeme_push(PcLexer *L, char c) {
  size_t n = L->lexeme_buf ? strlen(L->lexeme_buf) : 0;
  if (n + 2 > L->lexeme_cap) {
    L->lexeme_cap = L->lexeme_cap ? L->lexeme_cap * 2 : 32;
    while (n + 2 > L->lexeme_cap) L->lexeme_cap *= 2;
    L->lexeme_buf = realloc(L->lexeme_buf, L->lexeme_cap);
  }
  if (!L->lexeme_buf) return;
  L->lexeme_buf[n] = c;
  L->lexeme_buf[n + 1] = '\0';
}

static void lexeme_clear(PcLexer *L) {
  if (L->lexeme_buf) L->lexeme_buf[0] = '\0';
}

static unsigned char peek(const PcLexer *L, size_t off) {
  size_t p = L->pos + off;
  return p < L->len ? (unsigned char)L->src[p] : 0;
}

static void advance(PcLexer *L) {
  if (L->pos < L->len) {
    if (L->src[L->pos] == '\n') {
      L->line++;
      L->column = 1;
    } else
      L->column++;
    L->pos++;
  }
}

typedef struct Kw {
  const char *w;
  PcTokenKind k;
} Kw;

static const Kw KW[] = {
    {"declare", TOK_DECLARE},     {"constant", TOK_CONSTANT}, {"integer", TOK_INTEGER},
    {"real", TOK_REAL_T},         {"string", TOK_STRING_T},   {"char", TOK_CHAR_T},
    {"boolean", TOK_BOOLEAN},     {"date", TOK_DATE_T},       {"array", TOK_ARRAY},
    {"of", TOK_OF},               {"type", TOK_TYPE},         {"endtype", TOK_ENDTYPE},
    {"if", TOK_IF},               {"then", TOK_THEN},         {"else", TOK_ELSE},
    {"endif", TOK_ENDIF},         {"case", TOK_CASE},         {"endcase", TOK_ENDCASE},
    {"otherwise", TOK_OTHERWISE}, {"for", TOK_FOR},           {"to", TOK_TO},
    {"step", TOK_STEP},           {"next", TOK_NEXT},         {"repeat", TOK_REPEAT},
    {"until", TOK_UNTIL},         {"while", TOK_WHILE},       {"endwhile", TOK_ENDWHILE},
    {"procedure", TOK_PROCEDURE}, {"endprocedure", TOK_ENDPROCEDURE},
    {"function", TOK_FUNCTION},   {"endfunction", TOK_ENDFUNCTION},
    {"returns", TOK_RETURNS},     {"call", TOK_CALL},         {"return", TOK_RETURN},
    {"byval", TOK_BYVAL},         {"byref", TOK_BYREF},       {"input", TOK_INPUT},
    {"output", TOK_OUTPUT},       {"openfile", TOK_OPENFILE}, {"closefile", TOK_CLOSEFILE},
    {"readfile", TOK_READFILE},   {"writefile", TOK_WRITEFILE}, {"seek", TOK_SEEK},
    {"getrecord", TOK_GETRECORD}, {"putrecord", TOK_PUTRECORD},
    {"import", TOK_IMPORT},       {"as", TOK_AS},
    {"and", TOK_AND},             {"or", TOK_OR},             {"not", TOK_NOT},
    {"mod", TOK_MOD},             {"div", TOK_DIV},           {"true", TOK_TRUE},
    {"false", TOK_FALSE},
};

static PcTokenKind kw_lookup(char *low) {
  for (size_t i = 0; i < sizeof(KW) / sizeof(KW[0]); i++) {
    if (strcmp(low, KW[i].w) == 0) return KW[i].k;
  }
  return TOK_IDENT;
}

void pc_lex_init(PcLexer *L, const char *filename, const char *src, size_t len) {
  L->filename = filename;
  L->src = src;
  L->len = len;
  L->pos = 0;
  L->line = 1;
  L->column = 1;
  L->lexeme_buf = malloc(32);
  L->lexeme_cap = 32;
  if (L->lexeme_buf) L->lexeme_buf[0] = '\0';
  pc_lex_next(L);
}

void pc_lex_free(PcLexer *L) {
  free(L->lexeme_buf);
  L->lexeme_buf = NULL;
  L->lexeme_cap = 0;
}

static bool try_date_after_int(PcLexer *L, size_t start_pos, int start_line, int start_col) {
  size_t savep = L->pos;
  int sline = L->line, scol = L->column;
  if (peek(L, 0) != '/') return false;
  advance(L);
  size_t p2 = L->pos;
  while (isdigit(peek(L, 0))) advance(L);
  if (L->pos == p2) {
    L->pos = savep;
    L->line = sline;
    L->column = scol;
    return false;
  }
  if (peek(L, 0) != '/') {
    L->pos = savep;
    L->line = sline;
    L->column = scol;
    return false;
  }
  advance(L);
  size_t p3 = L->pos;
  int year_digits = 0;
  while (isdigit(peek(L, 0))) {
    year_digits++;
    advance(L);
  }
  if (year_digits != 4 || L->pos == p3) {
    L->pos = savep;
    L->line = sline;
    L->column = scol;
    return false;
  }
  L->cur.kind = TOK_DATE;
  L->cur.lexeme = L->src + start_pos;
  L->cur.lexeme_len = L->pos - start_pos;
  L->cur.line = start_line;
  L->cur.column = start_col;
  return true;
}

void pc_lex_next(PcLexer *L) {
again:
  while (L->pos < L->len && isspace((unsigned char)L->src[L->pos])) {
    if (L->src[L->pos] == '\n') {
      L->cur.kind = TOK_NEWLINE;
      L->cur.lexeme = L->src + L->pos;
      L->cur.lexeme_len = 1;
      L->cur.line = L->line;
      L->cur.column = L->column;
      advance(L);
      return;
    }
    advance(L);
  }
  if (L->pos >= L->len) {
    L->cur.kind = TOK_EOF;
    L->cur.lexeme = L->src + L->pos;
    L->cur.lexeme_len = 0;
    L->cur.line = L->line;
    L->cur.column = L->column;
    return;
  }

  int tline = L->line, tcol = L->column;
  size_t start = L->pos;
  unsigned char c = peek(L, 0);

  if (c == '/' && peek(L, 1) == '/') {
    while (L->pos < L->len && L->src[L->pos] != '\n') advance(L);
    goto again;
  }

  if (c == '"') {
    advance(L);
    lexeme_clear(L);
    while (L->pos < L->len && L->src[L->pos] != '"') {
      if (L->src[L->pos] == '\\' && L->pos + 1 < L->len) {
        advance(L);
        char e = L->src[L->pos];
        if (e == 'n')
          lexeme_push(L, '\n');
        else if (e == 't')
          lexeme_push(L, '\t');
        else if (e == 'r')
          lexeme_push(L, '\r');
        else if (e == '\\' || e == '"')
          lexeme_push(L, e);
        else
          lexeme_push(L, e);
        advance(L);
      } else {
        lexeme_push(L, L->src[L->pos]);
        advance(L);
      }
    }
    if (L->pos < L->len && L->src[L->pos] == '"') advance(L);
    L->cur.kind = TOK_STRING;
    L->cur.lexeme = L->lexeme_buf;
    L->cur.lexeme_len = L->lexeme_buf ? strlen(L->lexeme_buf) : 0;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  if (c == '\'') {
    advance(L);
    lexeme_clear(L);
    if (L->pos < L->len) {
      lexeme_push(L, L->src[L->pos]);
      advance(L);
    }
    if (L->pos < L->len && L->src[L->pos] == '\'') advance(L);
    L->cur.kind = TOK_CHAR;
    L->cur.lexeme = L->lexeme_buf;
    L->cur.lexeme_len = L->lexeme_buf ? strlen(L->lexeme_buf) : 0;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  if (isalpha((unsigned char)c) || c == '_') {
    lexeme_clear(L);
    while (L->pos < L->len &&
           (isalnum((unsigned char)L->src[L->pos]) || L->src[L->pos] == '_')) {
      lexeme_push(L, L->src[L->pos]);
      advance(L);
    }
    pc_str_tolower(L->lexeme_buf);
    L->cur.kind = kw_lookup(L->lexeme_buf);
    L->cur.lexeme = L->lexeme_buf;
    L->cur.lexeme_len = strlen(L->lexeme_buf);
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  if (isdigit((unsigned char)c)) {
    lexeme_clear(L);
    while (isdigit(peek(L, 0))) lexeme_push(L, L->src[L->pos]), advance(L);
    if (peek(L, 0) == '.' && isdigit(peek(L, 1))) {
      lexeme_push(L, '.');
      advance(L);
      while (isdigit(peek(L, 0))) lexeme_push(L, L->src[L->pos]), advance(L);
      L->cur.kind = TOK_REAL;
      L->cur.lexeme = L->lexeme_buf;
      L->cur.lexeme_len = strlen(L->lexeme_buf);
      L->cur.line = tline;
      L->cur.column = tcol;
      return;
    }
    if (try_date_after_int(L, start, tline, tcol)) return;
    L->cur.kind = TOK_INT;
    L->cur.lexeme = L->lexeme_buf;
    L->cur.lexeme_len = strlen(L->lexeme_buf);
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  /* UTF-8 LEFTWARDS ARROW U+2190 */
  if (c == 0xe2 && peek(L, 1) == 0x86 && peek(L, 2) == 0x90) {
    advance(L);
    advance(L);
    advance(L);
    L->cur.kind = TOK_ASSIGN;
    L->cur.lexeme = L->src + start;
    L->cur.lexeme_len = 3;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  if (c == '<' && peek(L, 1) == '-') {
    advance(L);
    advance(L);
    L->cur.kind = TOK_ASSIGN;
    L->cur.lexeme = L->src + start;
    L->cur.lexeme_len = 2;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  if (c == '<' && peek(L, 1) == '=') {
    advance(L);
    advance(L);
    L->cur.kind = TOK_LE;
    L->cur.lexeme = L->src + start;
    L->cur.lexeme_len = 2;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }
  if (c == '>' && peek(L, 1) == '=') {
    advance(L);
    advance(L);
    L->cur.kind = TOK_GE;
    L->cur.lexeme = L->src + start;
    L->cur.lexeme_len = 2;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }
  if (c == '<' && peek(L, 1) == '>') {
    advance(L);
    advance(L);
    L->cur.kind = TOK_NE;
    L->cur.lexeme = L->src + start;
    L->cur.lexeme_len = 2;
    L->cur.line = tline;
    L->cur.column = tcol;
    return;
  }

  advance(L);
  switch (c) {
  case '=':
    L->cur.kind = TOK_EQ;
    break;
  case '<':
    L->cur.kind = TOK_LT;
    break;
  case '>':
    L->cur.kind = TOK_GT;
    break;
  case '(':
    L->cur.kind = TOK_LPAREN;
    break;
  case ')':
    L->cur.kind = TOK_RPAREN;
    break;
  case '[':
    L->cur.kind = TOK_LBRACK;
    break;
  case ']':
    L->cur.kind = TOK_RBRACK;
    break;
  case ',':
    L->cur.kind = TOK_COMMA;
    break;
  case ':':
    L->cur.kind = TOK_COLON;
    break;
  case '.':
    L->cur.kind = TOK_DOT;
    break;
  case '&':
    L->cur.kind = TOK_AMP;
    break;
  case '+':
    L->cur.kind = TOK_PLUS;
    break;
  case '-':
    L->cur.kind = TOK_MINUS;
    break;
  case '*':
    L->cur.kind = TOK_STAR;
    break;
  case '/':
    L->cur.kind = TOK_SLASH;
    break;
  default:
    L->cur.kind = TOK_ERROR;
    break;
  }
  L->cur.lexeme = L->src + start;
  L->cur.lexeme_len = L->pos - start;
  L->cur.line = tline;
  L->cur.column = tcol;
}

bool pc_lex_match(PcLexer *L, PcTokenKind k) {
  if (L->cur.kind == k) {
    pc_lex_next(L);
    return true;
  }
  return false;
}

void pc_lex_skip_newlines(PcLexer *L) {
  while (L->cur.kind == TOK_NEWLINE) pc_lex_next(L);
}

void pc_lex_save(const PcLexer *L, PcLexState *s) {
  s->pos = L->pos;
  s->line = L->line;
  s->column = L->column;
  s->cur = L->cur;
  s->has_lexeme_snap = false;
  if (L->lexeme_buf && L->cur.lexeme == L->lexeme_buf && L->cur.lexeme_len < sizeof(s->lexeme_snap)) {
    memcpy(s->lexeme_snap, L->cur.lexeme, L->cur.lexeme_len);
    s->lexeme_snap[L->cur.lexeme_len] = '\0';
    s->has_lexeme_snap = true;
  }
}

void pc_lex_restore(PcLexer *L, const PcLexState *s) {
  L->pos = s->pos;
  L->line = s->line;
  L->column = s->column;
  L->cur = s->cur;
  if (s->has_lexeme_snap && L->lexeme_buf) {
    memcpy(L->lexeme_buf, s->lexeme_snap, s->cur.lexeme_len);
    L->lexeme_buf[s->cur.lexeme_len] = '\0';
    L->cur.lexeme = L->lexeme_buf;
  }
}
