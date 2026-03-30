#include "pc_pkg_shared.h"
#include "pc_common.h"
#include "pc_error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(p) mkdir(p, 0755)
#endif

#include <dirent.h>

static char *pkg_root(void) {
#ifdef _WIN32
  const char *home = getenv("USERPROFILE");
#else
  const char *home = getenv("HOME");
#endif
  if (!home) home = ".";
  size_t n = strlen(home) + 32;
  char *p = malloc(n);
  if (!p) return NULL;
  snprintf(p, n, "%s/.pseudocode", home);
  return p;
}

static void ensure_dir(const char *path) {
  MKDIR(path);
}

static char *join_path(const char *a, const char *b) {
  size_t na = strlen(a), nb = strlen(b);
  char *r = malloc(na + nb + 2);
  if (!r) return NULL;
  memcpy(r, a, na);
  r[na] = '/';
  memcpy(r + na + 1, b, nb + 1);
  return r;
}

static int copy_file(const char *src, const char *dst, PcErrorCtx *err) {
  FILE *in = fopen(src, "rb");
  if (!in) {
    pc_io_error(err, src, PC_ERR_IO_FILE, "cannot read file");
    return -1;
  }
  FILE *out = fopen(dst, "wb");
  if (!out) {
    fclose(in);
    pc_io_error(err, dst, PC_ERR_IO_FILE, "cannot write file");
    return -1;
  }
  char buf[8192];
  size_t n;
  while ((n = fread(buf, 1, sizeof buf, in)) > 0) fwrite(buf, 1, n, out);
  fclose(in);
  fclose(out);
  return 0;
}

int pc_pkg_install_local(const char *src_dir, const char *pkg_name, PcErrorCtx *err) {
  char *root = pkg_root();
  if (!root) return -1;
  ensure_dir(root);
  char *pkgs = join_path(root, "packages");
  ensure_dir(pkgs);
  char *dest = join_path(pkgs, pkg_name);
  ensure_dir(dest);
  DIR *d = opendir(src_dir);
  if (!d) {
    pc_note(err, "package path not found: %s", src_dir);
    free(dest);
    free(pkgs);
    free(root);
    return -1;
  }
  struct dirent *e;
  while ((e = readdir(d))) {
    if (e->d_name[0] == '.') continue;
    size_t L = strlen(e->d_name);
    bool ok = (L > 9 && strcmp(e->d_name + L - 10, ".pseudocode") == 0) ||
              (L > 6 && strcmp(e->d_name + L - 7, ".pseudo") == 0);
    if (!ok) continue;
    char *sp = join_path(src_dir, e->d_name);
    char *dp = join_path(dest, e->d_name);
    copy_file(sp, dp, err);
    free(dp);
    free(sp);
  }
  closedir(d);
  printf("Installed package '%s' into %s\n", pkg_name, dest);
  free(dest);
  free(pkgs);
  free(root);
  return 0;
}

int pc_pkg_list(void) {
  char *root = pkg_root();
  if (!root) return -1;
  char *pkgs = join_path(root, "packages");
  DIR *d = opendir(pkgs);
  if (!d) {
    printf("(no packages installed)\n");
    free(pkgs);
    free(root);
    return 0;
  }
  struct dirent *e;
  while ((e = readdir(d))) {
    if (e->d_name[0] == '.') continue;
    printf("%s\n", e->d_name);
  }
  closedir(d);
  free(pkgs);
  free(root);
  return 0;
}

int pc_pkg_remove(const char *name) {
  char *root = pkg_root();
  char *pkgs = join_path(root, "packages");
  char *dest = join_path(pkgs, name);
  char cmd[1024];
#ifdef _WIN32
  snprintf(cmd, sizeof cmd, "rmdir /S /Q \"%s\"", dest);
#else
  snprintf(cmd, sizeof cmd, "rm -rf \"%s\"", dest);
#endif
  int r = system(cmd);
  free(dest);
  free(pkgs);
  free(root);
  return r;
}

void pc_pkg_print_registry_note(void) {
  char *root = pkg_root();
  if (!root) return;
  printf("Local package root: %s/packages\n", root);
  printf("Registry URL (optional): set in %s/registry.txt — one URL per line.\n", root);
  free(root);
}
