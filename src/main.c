#include "pc_ast.h"
#include "pc_common.h"
#include "pc_error.h"
#include "pc_interp.h"
#include "pc_lexer.h"
#include "pc_parser.h"
#include "pc_repl.h"
#include "pc_welcome.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *PC_EXAMPLE_SOURCE = "OUTPUT \"Welcome to Pseudocode!\"\n"
                                        "DECLARE name : STRING\n"
                                        "INPUT name\n"
                                        "OUTPUT \"Hello \" & name\n";

static char *read_all(const char *path, size_t *out_len) {
  FILE *f = fopen(path, "rb");
  if (!f) return NULL;
  fseek(f, 0, SEEK_END);
  long n = ftell(f);
  if (n < 0) {
    fclose(f);
    return NULL;
  }
  fseek(f, 0, SEEK_SET);
  char *b = malloc((size_t)n + 1);
  if (!b) {
    fclose(f);
    return NULL;
  }
  fread(b, 1, (size_t)n, f);
  b[n] = '\0';
  fclose(f);
  if (out_len) *out_len = (size_t)n;
  return b;
}

static bool ends_with(const char *s, const char *suf) {
  size_t ns = strlen(s), nf = strlen(suf);
  return ns >= nf && strcmp(s + (ns - nf), suf) == 0;
}

static int run_source(const char *src, size_t len, const char *path_label, bool check_only, bool trace,
                      bool color) {
  PcErrorCtx err;
  pc_err_init(&err, trace, color);
  PcLexer L;
  pc_lex_init(&L, path_label, src, len);
  PcAst *prog = pc_parse_program(&L, &err);
  pc_lex_free(&L);
  if (!prog || err.had_error) {
    pc_ast_free(prog);
    return 1;
  }
  if (check_only) {
    Interp *I = pc_interp_new(&err);
    bool ok = pc_interp_check(I, prog);
    pc_interp_free(I);
    pc_ast_free(prog);
    if (ok) printf("check: OK (parsed successfully)\n");
    return ok ? 0 : 1;
  }
  Interp *I = pc_interp_new(&err);
  pc_interp_set_trace(I, trace);
  pc_interp_run(I, prog);
  pc_interp_free(I);
  pc_ast_free(prog);
  return err.had_error ? 1 : 0;
}

static int run_file(const char *path, bool check_only, bool trace, bool color) {
  size_t len = 0;
  char *src = read_all(path, &len);
  if (!src) {
    fprintf(stderr, "cannot read file: %s\n", path);
    return 1;
  }
  int r = run_source(src, len, path, check_only, trace, color);
  free(src);
  return r;
}

static void help(bool color) {
  (void)color;
  printf("pseudocode v%s — command reference\n", PC_VERSION_STRING);
  printf("Run pseudocode with no arguments for the full welcome screen.\n\n");
  printf("Commands:\n");
  printf("  pseudocode <file.pseudo>       Run a program\n");
  printf("  pseudocode run <file>          Same as above\n");
  printf("  pseudocode example             Run built-in welcome demo (stdin: your name)\n");
  printf("  pseudocode check <file>        Parse and validate structure\n");
  printf("  pseudocode repl                Interactive REPL (EXIT / quit / :quit)\n");
  printf("  pseudocode version             Print version\n");
  printf("  pseudocode help                Show this help\n\n");
  printf("Environment:\n");
  printf("  PSEUDO_TRACE=1                 Execution trace on stderr\n");
  printf("  PSEUDO_NO_COLOR=1              Disable ANSI colours\n");
}

int main(int argc, char **argv) {
  bool color = getenv("PSEUDO_NO_COLOR") == NULL;
  bool trace = getenv("PSEUDO_TRACE") != NULL;
  if (argc < 2) {
    pc_welcome_print(stdout, color);
    return 0;
  }
  const char *a1 = argv[1];
  if (strcmp(a1, "help") == 0 || strcmp(a1, "-h") == 0 || strcmp(a1, "--help") == 0) {
    help(color);
    return 0;
  }
  if (strcmp(a1, "version") == 0 || strcmp(a1, "-v") == 0) {
    printf("pseudocode %s\n", PC_VERSION_STRING);
    return 0;
  }
  if (strcmp(a1, "repl") == 0) {
    PcErrorCtx err;
    pc_err_init(&err, trace, color);
    return pc_repl_run(&err, trace);
  }
  if (strcmp(a1, "example") == 0) {
    size_t n = strlen(PC_EXAMPLE_SOURCE);
    return run_source(PC_EXAMPLE_SOURCE, n, "<example>", false, trace, color);
  }
  if (strcmp(a1, "run") == 0) {
    if (argc < 3) {
      help(color);
      return 1;
    }
    return run_file(argv[2], false, trace, color);
  }
  if (strcmp(a1, "check") == 0) {
    if (argc < 3) {
      help(color);
      return 1;
    }
    return run_file(argv[2], true, false, color);
  }
  if (ends_with(a1, ".pseudo") || ends_with(a1, ".pseudocode")) {
    return run_file(a1, false, trace, color);
  }
  help(color);
  fprintf(stderr, "unknown command or file: %s\n", a1);
  return 1;
}
