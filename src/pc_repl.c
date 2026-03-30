#include "pc_repl.h"
#include "pc_ast.h"
#include "pc_interp.h"
#include "pc_lexer.h"
#include "pc_parser.h"
#include "pc_common.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool repl_is_quit(const char *ln) {
  if (!ln) return true;
  while (*ln == ' ' || *ln == '\t') ln++;
  size_t n = strlen(ln);
  while (n > 0 && (ln[n - 1] == '\n' || ln[n - 1] == '\r')) n--;
  if (n == 0) return false;
  char buf[32];
  if (n >= sizeof buf) return false;
  memcpy(buf, ln, n);
  buf[n] = '\0';
  for (size_t i = 0; i < n; i++) buf[i] = (char)tolower((unsigned char)buf[i]);
  return strcmp(buf, ":quit") == 0 || strcmp(buf, "exit") == 0 || strcmp(buf, "quit") == 0;
}

#ifdef PC_HAVE_READLINE
#include <readline/history.h>
#include <readline/readline.h>
#endif

static char *slurp_stdin_line(void) {
#ifdef PC_HAVE_READLINE
  char *ln = readline("pseudo> ");
  if (ln && *ln) add_history(ln);
  return ln;
#else
  char buf[4096];
  if (!fgets(buf, sizeof buf, stdin)) return NULL;
  return pc_strdup(buf);
#endif
}

static char *buf_append(char *buf, const char *s) {
  size_t a = buf ? strlen(buf) : 0, b = strlen(s);
  char *n = realloc(buf, a + b + 1);
  if (!n) return buf;
  memcpy(n + a, s, b + 1);
  return n;
}

int pc_repl_run(PcErrorCtx *err, bool trace) {
  static Interp *I = NULL;
  if (!I) I = pc_interp_new(err);
  pc_interp_set_trace(I, trace);
  printf("Pseudocode REPL — type lines of code; blank line runs the buffer.\n");
  printf("Exit with EXIT, quit, or :quit.\n");
  char *acc = pc_strdup("");
  for (;;) {
    char *ln = slurp_stdin_line();
    if (!ln) break;
    if (repl_is_quit(ln)) {
      free(ln);
      break;
    }
    if (ln[0] == '\n' || ln[0] == '\0') {
      free(ln);
      if (!acc || !acc[0]) continue;
      size_t len = strlen(acc);
      PcLexer L;
      pc_lex_init(&L, "<repl>", acc, len);
      PcAst *prog = pc_parse_program(&L, err);
      pc_lex_free(&L);
      if (prog && !err->had_error) {
        pc_interp_run(I, prog);
        pc_ast_free(prog);
      } else if (prog)
        pc_ast_free(prog);
      free(acc);
      acc = pc_strdup("");
      err->had_error = false;
      continue;
    }
    acc = buf_append(acc, ln);
    free(ln);
  }
  free(acc);
  return 0;
}
