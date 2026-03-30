#ifndef PC_VALUE_H
#define PC_VALUE_H

#include "pc_error.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum PcTypeKind {
  PC_T_VOID,
  PC_T_INT,
  PC_T_REAL,
  PC_T_BOOL,
  PC_T_CHAR,
  PC_T_STRING,
  PC_T_DATE,
  PC_T_ARRAY,
} PcTypeKind;

typedef struct PcType {
  PcTypeKind kind;
  struct PcType *elem;
  int lo0, hi0, lo1, hi1;
  bool is2d;
} PcType;

typedef struct PcValue PcValue;
struct PcValue {
  PcType *type;
  int64_t i;
  double r;
  bool b;
  char c;
  char *s;
  PcValue *cells;
  bool owned_str;
};

void pc_type_free(PcType *t);
PcType *pc_type_clone(const PcType *t);

void pc_value_init_void(PcValue *v);
void pc_value_init_int(PcValue *v, int64_t x);
void pc_value_init_real(PcValue *v, double x);
void pc_value_init_bool(PcValue *v, bool x);
void pc_value_init_char(PcValue *v, char x);
void pc_value_init_string(PcValue *v, char *s, bool take_ownership);
void pc_value_init_date(PcValue *v, const char *date_str);
void pc_value_init_array(PcValue *v, PcType *arr_type, PcValue *cells);

void pc_value_destroy(PcValue *v);
void pc_value_copy(PcValue *dst, const PcValue *src);

bool pc_value_truthy(const PcValue *v);
const char *pc_value_type_name(const PcValue *v);

#endif
