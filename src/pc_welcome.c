#include "pc_welcome.h"
#include "pc_common.h"

#ifdef _WIN32
#include <io.h>
#define OUT_TTY(fp) _isatty(_fileno(fp))
#else
#include <unistd.h>
#define OUT_TTY(fp) isatty(fileno(fp))
#endif

static void S(FILE *o, bool color, const char *ansi) {
  if (color && OUT_TTY(o)) fputs(ansi, o);
}

static void R(FILE *o, bool color) {
  S(o, color, "\033[0m");
}

void pc_welcome_print(FILE *out, bool color) {
  const char *dim = "\033[2m";
  const char *b = "\033[1m";
  const char *cyan = "\033[36m";
  const char *grn = "\033[32m";
  const char *w = "\033[97m";
  const char *mag = "\033[35m";

  /* Outer frame (fixed width, single-byte punctuation where possible) */
  S(out, color, cyan);
  fputs("\n  ", out);
  for (int i = 0; i < 62; i++) fputs("\xe2\x94\x80", out); /* ─ */
  fputs("\n", out);
  R(out, color);

  S(out, color, dim);
  fputs("  ", out);
  fputs("/*", out);
  for (int i = 0; i < 56; i++) fputc(' ', out);
  fputs("*/\n", out);
  R(out, color);

  S(out, color, cyan);
  fputs("  ", out);
  fputs("\xe2\x95\x90", out);
  fputs("\xe2\x95\x90", out);
  fputs("\xe2\x95\x90", out);
  S(out, color, w);
  S(out, color, b);
  fputs("  P S E U D O C O D E  ", out);
  R(out, color);
  S(out, color, cyan);
  fputs("\xe2\x95\x90", out);
  fputs("\xe2\x95\x90", out);
  fputs("\xe2\x95\x90", out);
  fputs("\n", out);
  R(out, color);

  S(out, color, dim);
  fputs("  ", out);
  fputs("/*", out);
  for (int i = 0; i < 56; i++) fputc(' ', out);
  fputs("*/\n", out);
  R(out, color);

  S(out, color, cyan);
  fputs("  ", out);
  for (int i = 0; i < 62; i++) fputs("\xe2\x94\x80", out);
  fputs("\n\n", out);
  R(out, color);

  /* Large wordmark (no side borders — avoids terminal double-width alignment issues) */
  S(out, color, w);
  S(out, color, b);
  fputs("    ██████╗ ███████╗███████╗██╗   ██╗██████╗  ██████╗\n", out);
  fputs("    ██╔══██╗██╔════╝██╔════╝██║   ██║██╔══██╗██╔═══██╗\n", out);
  fputs("    ██████╔╝█████╗  ███████╗██║   ██║██║  ██║██║   ██║\n", out);
  fputs("    ██╔═══╝ ██╔══╝  ╚════██║██║   ██║██║  ██║██║   ██║\n", out);
  fputs("    ██║     ███████╗███████║╚██████╔╝██████╔╝╚██████╔╝\n", out);
  fputs("    ╚═╝     ╚══════╝╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝\n", out);
  fputs("\n", out);
  fputs("     ██████╗ ██████╗ ██████╗ ███████╗\n", out);
  fputs("    ██╔════╝██╔═══██╗██╔══██╗██╔════╝\n", out);
  fputs("    ██║     ██║   ██║██║  ██║█████╗\n", out);
  fputs("    ██║     ██║   ██║██║  ██║██╔══╝\n", out);
  fputs("    ╚██████╗╚██████╔╝██████╔╝███████╗\n", out);
  fputs("     ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝\n", out);
  R(out, color);

  S(out, color, cyan);
  fputs("\n  ", out);
  for (int i = 0; i < 62; i++) fputs("\xe2\x94\x80", out);
  fputs("\n", out);
  R(out, color);

  S(out, color, dim);
  fputs("\n                          by Mahesh Ravariya\n", out);
  R(out, color);

  fputs("\n  ", out);
  S(out, color, mag);
  fputs("\xe2\x97\x86 ", out); /* ◆ */
  R(out, color);
  fputs("Pseudocode ", out);
  S(out, color, b);
  fputs("v", out);
  fputs(PC_VERSION_STRING, out);
  R(out, color);
  fputs("\n  ", out);
  S(out, color, mag);
  fputs("\xe2\x97\x86 ", out);
  R(out, color);
  fputs("Package manager ", out);
  S(out, color, b);
  fputs("pkg v", out);
  fputs(PC_VERSION_STRING, out);
  R(out, color);
  fputs("\n", out);

  fputs("\n  ", out);
  S(out, color, cyan);
  fputs("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80 ", out);
  R(out, color);
  S(out, color, b);
  fputs("Getting started", out);
  R(out, color);
  S(out, color, cyan);
  fputs(" \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\n", out);
  R(out, color);

  fputs("\n", out);
  S(out, color, grn);
  fputs("    pseudocode", out);
  R(out, color);
  fputs(" main.pseudo          ", out);
  S(out, color, dim);
  fputs("# run a program\n", out);
  R(out, color);
  S(out, color, grn);
  fputs("    pseudocode", out);
  R(out, color);
  fputs(" example              ", out);
  S(out, color, dim);
  fputs("# run the built-in welcome demo\n", out);
  R(out, color);
  S(out, color, grn);
  fputs("    pseudocode", out);
  R(out, color);
  fputs(" repl                 ", out);
  S(out, color, dim);
  fputs("# interactive mode (REPL)\n", out);
  R(out, color);
  S(out, color, grn);
  fputs("    pkg", out);
  R(out, color);
  fputs(" install <path>       ", out);
  S(out, color, dim);
  fputs("# install local package folder\n", out);
  R(out, color);

  fputs("\n  ", out);
  S(out, color, cyan);
  fputs("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80 ", out);
  R(out, color);
  S(out, color, b);
  fputs("Interactive mode (REPL)", out);
  R(out, color);
  S(out, color, cyan);
  fputs(" \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\n", out);
  R(out, color);
  fputs("\n", out);
  S(out, color, grn);
  fputs("    pseudocode repl\n", out);
  R(out, color);
  fputs("\n", out);
  S(out, color, dim);
  fputs("    * Run commands line by line; blank line executes the buffer.\n"
        "    * Test logic instantly; no file needed.\n"
        "    * Type ",
        out);
  S(out, color, grn);
  fputs("EXIT", out);
  S(out, color, dim);
  fputs(", ", out);
  S(out, color, grn);
  fputs("quit", out);
  S(out, color, dim);
  fputs(", or ", out);
  S(out, color, grn);
  fputs(":quit", out);
  S(out, color, dim);
  fputs(" to leave.\n", out);
  R(out, color);

  fputs("\n  ", out);
  S(out, color, cyan);
  fputs("\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80 ", out);
  R(out, color);
  S(out, color, b);
  fputs("Sample program", out);
  R(out, color);
  S(out, color, cyan);
  fputs(" \xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\n", out);
  R(out, color);
  fputs("\n", out);
  pc_welcome_print_example_snippet(out, color);

  fputs("\n  ", out);
  S(out, color, dim);
  fputs("More: ", out);
  S(out, color, grn);
  fputs("pseudocode help", out);
  S(out, color, dim);
  fputs("  *  ", out);
  S(out, color, grn);
  fputs("pseudocode version", out);
  S(out, color, dim);
  fputs("  *  ", out);
  S(out, color, grn);
  fputs("pseudocode check <file>", out);
  R(out, color);
  fputs("\n\n", out);
}

void pc_welcome_print_example_snippet(FILE *out, bool color) {
  const char *dim = "\033[2m";
  const char *ylw = "\033[33m";
  S(out, color, dim);
  fputs("    +--\n", out);
  R(out, color);
  S(out, color, ylw);
  fputs("    |  OUTPUT \"Welcome to Pseudocode!\"\n"
        "    |  DECLARE name : STRING\n"
        "    |  INPUT name\n"
        "    |  OUTPUT \"Hello \" & name\n",
        out);
  R(out, color);
  S(out, color, dim);
  fputs("    +--\n", out);
  R(out, color);
}
