#ifndef PC_COMMON_H
#define PC_COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define PC_VERSION_MAJOR 1
#define PC_VERSION_MINOR 0
#define PC_VERSION_PATCH 0
#define PC_VERSION_STRING "1.0.0"
/* pkg shares the same release version as the pseudocode interpreter. */
#define PC_PKG_VERSION_STRING PC_VERSION_STRING

/* Maximum source file size read by the CLI (denies accidentally huge inputs). */
#define PC_MAX_SOURCE_BYTES (32u * 1024u * 1024u)

#define PC_ARRAY_GROW(a, cap, len, item_size) \
  do { \
    if ((len) >= (cap)) { \
      size_t _nc = (cap) ? (cap) * 2 : 8; \
      void *_p = realloc((a), _nc * (item_size)); \
      if (!_p) break; \
      (a) = _p; \
      (cap) = _nc; \
    } \
  } while (0)

char *pc_strdup(const char *s);
char *pc_strndup(const char *s, size_t n);
void pc_str_tolower(char *s);
int pc_utf8_len(const char *s, size_t nbytes);
bool pc_str_eq_ci(const char *a, const char *b);

#endif
