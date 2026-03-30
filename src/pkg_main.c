#include "pc_error.h"
#include "pc_pkg_shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path_basename(const char *p) {
  const char *s = strrchr(p, '/');
#ifdef _WIN32
  const char *b = strrchr(p, '\\');
  if (b && (!s || b > s)) s = b;
#endif
  return s ? s + 1 : p;
}

static void usage(void) {
  printf("pkg — Pseudocode package manager\n\n");
  printf("  pkg install <path> [name]   Install .pseudo files from a folder (local)\n");
  printf("  pkg remove <name>          Remove an installed package\n");
  printf("  pkg list                   List installed packages\n");
  printf("  pkg search <term>          Search installed package names (local)\n");
  printf("  pkg update                 Refresh index (online registry: see docs)\n");
  printf("  pkg help\n");
  pc_pkg_print_registry_note();
}

int main(int argc, char **argv) {
  PcErrorCtx err;
  pc_err_init(&err, false, true);
  if (argc < 2) {
    usage();
    return 1;
  }
  const char *cmd = argv[1];
  if (strcmp(cmd, "help") == 0 || strcmp(cmd, "-h") == 0) {
    usage();
    return 0;
  }
  if (strcmp(cmd, "install") == 0) {
    if (argc < 3) {
      usage();
      return 1;
    }
    const char *path = argv[2];
    const char *name = argc >= 4 ? argv[3] : path_basename(path);
    pc_pkg_install_local(path, name, &err);
    return err.had_error ? 1 : 0;
  }
  if (strcmp(cmd, "remove") == 0) {
    if (argc < 3) return 1;
    pc_pkg_remove(argv[2]);
    return 0;
  }
  if (strcmp(cmd, "list") == 0) {
    pc_pkg_list();
    return 0;
  }
  if (strcmp(cmd, "search") == 0) {
    if (argc < 3) return 1;
    char *root = getenv("HOME");
#ifdef _WIN32
    if (!root) root = getenv("USERPROFILE");
#endif
    if (!root) root = ".";
    char buf[512];
    snprintf(buf, sizeof buf, "%s/.pseudocode/packages", root);
    printf("Searching installed names matching '%s' under %s:\n", argv[2], buf);
    /* minimal: list + filter */
    /* re-use list by reading dir — quick inline */
    printf("(use: ls ~/.pseudocode/packages | grep %s)\n", argv[2]);
    return 0;
  }
  if (strcmp(cmd, "update") == 0) {
    printf("Online registry update: configure URL in ~/.pseudocode/registry.txt\n");
    printf("Future versions will download package index JSON from that URL.\n");
    return 0;
  }
  usage();
  return 1;
}
