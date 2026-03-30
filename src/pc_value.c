#include "pc_value.h"
#include "pc_common.h"
#include <stdlib.h>
#include <string.h>

void pc_type_free(PcType *t) {
  if (!t) return;
  if (t->elem) pc_type_free(t->elem);
  free(t);
}

PcType *pc_type_clone(const PcType *t) {
  if (!t) return NULL;
  PcType *n = malloc(sizeof(PcType));
  if (!n) return NULL;
  *n = *t;
  n->elem = t->elem ? pc_type_clone(t->elem) : NULL;
  return n;
}

void pc_value_init_void(PcValue *v) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_VOID;
    v->type->elem = NULL;
  }
}

void pc_value_init_int(PcValue *v, int64_t x) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_INT;
    v->type->elem = NULL;
  }
  v->i = x;
}

void pc_value_init_real(PcValue *v, double x) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_REAL;
    v->type->elem = NULL;
  }
  v->r = x;
}

void pc_value_init_bool(PcValue *v, bool x) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_BOOL;
    v->type->elem = NULL;
  }
  v->b = x;
}

void pc_value_init_char(PcValue *v, char x) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_CHAR;
    v->type->elem = NULL;
  }
  v->c = x;
}

void pc_value_init_string(PcValue *v, char *s, bool take_ownership) {
  memset(v, 0, sizeof(*v));
  v->type = malloc(sizeof(PcType));
  if (v->type) {
    v->type->kind = PC_T_STRING;
    v->type->elem = NULL;
  }
  v->s = s;
  v->owned_str = take_ownership;
}

void pc_value_init_date(PcValue *v, const char *date_str) {
  pc_value_init_string(v, pc_strdup(date_str), true);
  if (v->type) v->type->kind = PC_T_DATE;
}

void pc_value_init_array(PcValue *v, PcType *arr_type, PcValue *cells) {
  memset(v, 0, sizeof(*v));
  v->type = arr_type;
  v->cells = cells;
}

void pc_value_destroy(PcValue *v) {
  if (!v) return;
  if (v->type && v->type->kind == PC_T_ARRAY && v->cells) {
    int n = (v->type->hi0 - v->type->lo0 + 1);
    if (v->type->is2d) n *= (v->type->hi1 - v->type->lo1 + 1);
    for (int i = 0; i < n; i++) pc_value_destroy(&v->cells[i]);
    free(v->cells);
  }
  if (v->owned_str && v->s) free(v->s);
  pc_type_free(v->type);
  v->type = NULL;
  v->s = NULL;
  v->cells = NULL;
}

void pc_value_copy(PcValue *dst, const PcValue *src) {
  if (!src->type) {
    pc_value_init_void(dst);
    return;
  }
  switch (src->type->kind) {
  case PC_T_INT:
    pc_value_init_int(dst, src->i);
    break;
  case PC_T_REAL:
    pc_value_init_real(dst, src->r);
    break;
  case PC_T_BOOL:
    pc_value_init_bool(dst, src->b);
    break;
  case PC_T_CHAR:
    pc_value_init_char(dst, src->c);
    break;
  case PC_T_STRING:
  case PC_T_DATE:
    pc_value_init_string(dst, pc_strdup(src->s), true);
    if (dst->type) dst->type->kind = src->type->kind;
    break;
  case PC_T_ARRAY: {
    PcType *t = pc_type_clone(src->type);
    int n0 = src->type->hi0 - src->type->lo0 + 1;
    int n1 = src->type->is2d ? (src->type->hi1 - src->type->lo1 + 1) : 1;
    int n = n0 * n1;
    PcValue *cells = calloc((size_t)n, sizeof(PcValue));
    for (int i = 0; i < n; i++) pc_value_copy(&cells[i], &src->cells[i]);
    pc_value_init_array(dst, t, cells);
    break;
  }
  default:
    pc_value_init_void(dst);
    break;
  }
}

bool pc_value_truthy(const PcValue *v) {
  if (!v->type) return false;
  switch (v->type->kind) {
  case PC_T_BOOL:
    return v->b;
  case PC_T_INT:
    return v->i != 0;
  case PC_T_REAL:
    return v->r != 0.0;
  case PC_T_STRING:
  case PC_T_DATE:
    return v->s && v->s[0];
  case PC_T_CHAR:
    return v->c != 0;
  default:
    return false;
  }
}

const char *pc_value_type_name(const PcValue *v) {
  if (!v->type) return "VOID";
  switch (v->type->kind) {
  case PC_T_INT:
    return "INTEGER";
  case PC_T_REAL:
    return "REAL";
  case PC_T_BOOL:
    return "BOOLEAN";
  case PC_T_CHAR:
    return "CHAR";
  case PC_T_STRING:
    return "STRING";
  case PC_T_DATE:
    return "DATE";
  case PC_T_ARRAY:
    return "ARRAY";
  default:
    return "UNKNOWN";
  }
}
