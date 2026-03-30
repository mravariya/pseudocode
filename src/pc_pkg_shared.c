#include "pc_pkg_shared.h"
#include "pc_common.h"
#include "pc_error_codes.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define MKDIR(p) _mkdir(p)
#define PC_GETCWD(b, n) _getcwd(b, (int)(n))
#else
#include <unistd.h>
#define MKDIR(p) mkdir(p, 0755)
#define PC_GETCWD(b, n) getcwd(b, n)
#endif

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

static bool is_directory(const char *path) {
  struct stat st;
  if (stat(path, &st) != 0)
    return false;
#ifdef S_ISDIR
  return S_ISDIR(st.st_mode) != 0;
#else
  return (st.st_mode & S_IFDIR) != 0;
#endif
}

/* Search order: PSEUDOCODE_STDLIB, PC_BUILTIN_STDLIB_PACKAGES, cwd/stdlib-packages, ../stdlib-packages */
static char *find_stdlib_packages_root(void) {
  const char *e = getenv("PSEUDOCODE_STDLIB");
  if (e && e[0] && is_directory(e)) {
    return pc_strdup(e);
  }
#ifdef PC_BUILTIN_STDLIB_PACKAGES
  if (is_directory(PC_BUILTIN_STDLIB_PACKAGES))
    return pc_strdup(PC_BUILTIN_STDLIB_PACKAGES);
#endif
  char cwd[4096];
  if (!PC_GETCWD(cwd, sizeof cwd))
    return NULL;
  char *try1 = join_path(cwd, "stdlib-packages");
  if (try1 && is_directory(try1)) {
    char *r = pc_strdup(try1);
    free(try1);
    return r;
  }
  free(try1);
  char *parent = join_path(cwd, "..");
  if (!parent)
    return NULL;
  char *try2 = join_path(parent, "stdlib-packages");
  free(parent);
  if (try2 && is_directory(try2)) {
    char *r = pc_strdup(try2);
    free(try2);
    return r;
  }
  free(try2);
  return NULL;
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

void pc_pkg_print_version(void) {
  printf("pkg %s (Pseudocode toolkit; same version as the pseudocode interpreter)\n", PC_PKG_VERSION_STRING);
}

int pc_pkg_list_available(void) {
  char *cat = find_stdlib_packages_root();
  if (!cat) {
    printf(
        "(no stdlib catalog found — set PSEUDOCODE_STDLIB to the stdlib-packages directory, or run pkg from the "
        "repository root)\n");
    return 0;
  }
  printf("Catalog: %s\n", cat);
  DIR *d = opendir(cat);
  if (!d) {
    free(cat);
    return -1;
  }
  struct dirent *e;
  int n = 0;
  while ((e = readdir(d))) {
    if (e->d_name[0] == '.')
      continue;
    char *sub = join_path(cat, e->d_name);
    if (sub && is_directory(sub)) {
      printf("  %s\n", e->d_name);
      n++;
    }
    free(sub);
  }
  closedir(d);
  if (n == 0)
    printf("(empty catalog)\n");
  free(cat);
  return 0;
}

int pc_pkg_install_catalog(const char *pkg_name, PcErrorCtx *err) {
  if (!pkg_name || !pkg_name[0]) {
    pc_note(err, "missing package name");
    return -1;
  }
  char *cat = find_stdlib_packages_root();
  if (!cat) {
    pc_note(err, "stdlib catalog not found; set PSEUDOCODE_STDLIB or run from the repo (see: pkg available)");
    return -1;
  }
  char *src = join_path(cat, pkg_name);
  free(cat);
  if (!src || !is_directory(src)) {
    pc_note(err, "unknown catalog package '%s' — run: pkg available", pkg_name);
    free(src);
    return -1;
  }
  int r = pc_pkg_install_local(src, pkg_name, err);
  free(src);
  return r;
}
