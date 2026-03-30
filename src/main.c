#include "pc_ast.h"
#include "pc_common.h"
#include "pc_error.h"
#include "pc_error_codes.h"
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

static int exit_for_errors(const PcErrorCtx *err) {
  if (!err->had_error)
    return 0;
  switch (err->kind) {
  case PC_ERRK_PARSE:
    return 2;
  case PC_ERRK_RUNTIME:
    return 3;
  case PC_ERRK_IO:
    return 4;
  default:
    return 1;
  }
}

static char *read_all(const char *path, size_t *out_len) {
  FILE *f = fopen(path, "rb");
  if (!f)
    return NULL;
  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    return NULL;
  }
  long n = ftell(f);
  if (n < 0) {
    fclose(f);
    return NULL;
  }
  if ((unsigned long)n > PC_MAX_SOURCE_BYTES) {
    fclose(f);
    return NULL;
  }
  if (fseek(f, 0, SEEK_SET) != 0) {
    fclose(f);
    return NULL;
  }
  char *b = malloc((size_t)n + 1);
  if (!b) {
    fclose(f);
    return NULL;
  }
  size_t got = fread(b, 1, (size_t)n, f);
  fclose(f);
  if (got != (size_t)n) {
    free(b);
    return NULL;
  }
  b[n] = '\0';
  if (out_len)
    *out_len = (size_t)n;
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
    return exit_for_errors(&err);
  }
  if (check_only) {
    Interp *I = pc_interp_new(&err);
    bool ok = pc_interp_check(I, prog);
    pc_interp_free(I);
    pc_ast_free(prog);
    if (ok)
      printf("check: OK (parsed successfully)\n");
    return ok ? 0 : exit_for_errors(&err);
  }
  Interp *I = pc_interp_new(&err);
  pc_interp_set_trace(I, trace);
  pc_interp_run(I, prog);
  pc_interp_free(I);
  pc_ast_free(prog);
  return exit_for_errors(&err);
}

static int run_file(const char *path, bool check_only, bool trace, bool color) {
  size_t len = 0;
  char *src = read_all(path, &len);
  if (!src) {
    fprintf(stderr, "error [PC-%d] [io]: cannot read file (missing, too large >%u MiB, or I/O error): %s\n",
            PC_ERR_IO_FILE, (unsigned)(PC_MAX_SOURCE_BYTES / (1024u * 1024u)), path);
    return 4;
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
  printf("  pseudocode version             Print version (alias: -v, --version)\n");
  printf("  pseudocode help                Show this help (alias: -h, --help)\n\n");
  printf("Exit status: 0 success, 2 parse, 3 runtime, 4 I/O, 5 usage/unknown command (see docs/error-codes.md)\n\n");
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
  if (strcmp(a1, "version") == 0 || strcmp(a1, "-v") == 0 || strcmp(a1, "--version") == 0) {
    printf("pseudocode %s\n", PC_VERSION_STRING);
    return 0;
  }
  if (strcmp(a1, "repl") == 0) {
    PcErrorCtx err;
    pc_err_init(&err, trace, color);
    pc_repl_run(&err, trace);
    return 0;
  }
  if (strcmp(a1, "example") == 0) {
    size_t n = strlen(PC_EXAMPLE_SOURCE);
    return run_source(PC_EXAMPLE_SOURCE, n, "<example>", false, trace, color);
  }
  if (strcmp(a1, "run") == 0) {
    if (argc < 3) {
      help(color);
      return 5;
    }
    return run_file(argv[2], false, trace, color);
  }
  if (strcmp(a1, "check") == 0) {
    if (argc < 3) {
      help(color);
      return 5;
    }
    return run_file(argv[2], true, false, color);
  }
  if (ends_with(a1, ".pseudo") || ends_with(a1, ".pseudocode")) {
    return run_file(a1, false, trace, color);
  }
  help(color);
  fprintf(stderr, "error [PC-%d] [parse]: unknown command or file: %s\n", PC_ERR_PARSE_STMT, a1);
  return 5;
}
