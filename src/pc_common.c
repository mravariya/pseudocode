#include "pc_common.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *pc_strdup(const char *s) {
  if (!s) return NULL;
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (p) memcpy(p, s, n);
  return p;
}

char *pc_strndup(const char *s, size_t n) {
  if (!s) return NULL;
  size_t len = strnlen(s, n);
  char *p = malloc(len + 1);
  if (!p) return NULL;
  memcpy(p, s, len);
  p[len] = '\0';
  return p;
}

void pc_str_tolower(char *s) {
  for (; *s; ++s) *s = (char)tolower((unsigned char)*s);
}

int pc_utf8_len(const char *s, size_t nbytes) {
  int n = 0;
  size_t i = 0;
  while (i < nbytes && s[i]) {
    unsigned char c = (unsigned char)s[i];
    if (c < 0x80)
      i += 1;
    else if ((c & 0xe0) == 0xc0)
      i += 2;
    else if ((c & 0xf0) == 0xe0)
      i += 3;
    else if ((c & 0xf8) == 0xf0)
      i += 4;
    else
      i += 1;
    n++;
  }
  return n;
}

bool pc_str_eq_ci(const char *a, const char *b) {
  if (!a || !b) return a == b;
  while (*a && *b) {
    if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return false;
    a++;
    b++;
  }
  return *a == *b;
}
