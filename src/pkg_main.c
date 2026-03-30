#include "pc_common.h"
#include "pc_error.h"
#include "pc_pkg_shared.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *path_basename(const char *p) {
  const char *s = strrchr(p, '/');
#ifdef _WIN32
  const char *b = strrchr(p, '\\');
  if (b && (!s || b > s)) s = b;
#endif
  return s ? s + 1 : p;
}

static bool is_dir_path(const char *path) {
  struct stat st;
  if (stat(path, &st) != 0)
    return false;
#ifdef S_ISDIR
  return S_ISDIR(st.st_mode) != 0;
#else
  return (st.st_mode & S_IFDIR) != 0;
#endif
}

static void usage(void) {
  printf("pkg %s — Pseudocode package manager\n\n", PC_PKG_VERSION_STRING);
  printf("Commands:\n");
  printf("  pkg install <dir> [name]     Copy .pseudo files from a local folder\n");
  printf("  pkg install <catalog-name>    Install optional stdlib bundle (random, math, …)\n");
  printf("  pkg -i <catalog-name>         Same as: pkg install <catalog-name>\n");
  printf("  pkg --install <catalog-name> Same as above\n");
  printf("  pkg available                List catalog packages (not installed by default)\n");
  printf("  pkg remove <name>            Remove an installed package\n");
  printf("  pkg list                     List installed packages\n");
  printf("  pkg search <term>            Hint for filtering installed names\n");
  printf("  pkg update                   Registry refresh (see docs)\n");
  printf("  pkg version                  Print pkg / toolkit version\n");
  printf("  pkg -v, --version            Same as version\n");
  printf("  pkg help                     This help\n");
  printf("  pkg -h, --help               Same as help\n\n");
  printf("Environment:\n");
  printf("  PSEUDOCODE_STDLIB   Path to stdlib-packages directory (catalog source)\n\n");
  pc_pkg_print_registry_note();
}

int main(int argc, char **argv) {
  char **alloc_argv = NULL;
  if (argc >= 3 && (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--install") == 0)) {
    alloc_argv = (char **)malloc((size_t)(argc + 1) * sizeof(char *));
    if (!alloc_argv)
      return 1;
    alloc_argv[0] = argv[0];
    alloc_argv[1] = (char *)"install";
    for (int i = 2; i < argc; i++) alloc_argv[i] = argv[i];
    alloc_argv[argc] = NULL;
    argv = alloc_argv;
  }

  PcErrorCtx err;
  pc_err_init(&err, false, true);

  if (argc < 2) {
    usage();
    free(alloc_argv);
    return 1;
  }
  const char *cmd = argv[1];
  int exit_code = 0;

  if (strcmp(cmd, "help") == 0 || strcmp(cmd, "-h") == 0 || strcmp(cmd, "--help") == 0) {
    usage();
  } else if (strcmp(cmd, "version") == 0 || strcmp(cmd, "-v") == 0 || strcmp(cmd, "--version") == 0) {
    pc_pkg_print_version();
  } else if (strcmp(cmd, "available") == 0) {
    pc_pkg_list_available();
  } else if (strcmp(cmd, "install") == 0) {
    if (argc < 3) {
      usage();
      exit_code = 1;
    } else {
      const char *path_or_name = argv[2];
      const char *name = argc >= 4 ? argv[3] : path_basename(path_or_name);
      if (is_dir_path(path_or_name)) {
        pc_pkg_install_local(path_or_name, name, &err);
        exit_code = err.had_error ? 1 : 0;
      } else {
        pc_err_clear(&err);
        if (pc_pkg_install_catalog(path_or_name, &err) == 0 && !err.had_error)
          exit_code = 0;
        else {
          pc_err_clear(&err);
          pc_pkg_install_local(path_or_name, name, &err);
          exit_code = err.had_error ? 1 : 0;
        }
      }
    }
  } else if (strcmp(cmd, "remove") == 0) {
    if (argc < 3)
      exit_code = 1;
    else
      pc_pkg_remove(argv[2]);
  } else if (strcmp(cmd, "list") == 0) {
    pc_pkg_list();
  } else if (strcmp(cmd, "search") == 0) {
    if (argc < 3) {
      exit_code = 1;
    } else {
      char *root = getenv("HOME");
#ifdef _WIN32
      if (!root) root = getenv("USERPROFILE");
#endif
      if (!root) root = ".";
      char buf[512];
      snprintf(buf, sizeof buf, "%s/.pseudocode/packages", root);
      printf("Searching installed names matching '%s' under %s:\n", argv[2], buf);
      printf("(use: ls ~/.pseudocode/packages | grep %s)\n", argv[2]);
    }
  } else if (strcmp(cmd, "update") == 0) {
    printf("Online registry update: configure URL in ~/.pseudocode/registry.txt\n");
    printf("Future versions will download package index JSON from that URL.\n");
  } else {
    usage();
    exit_code = 1;
  }

  free(alloc_argv);
  return exit_code;
}
