#include "pc_stdlib.h"
#include "pc_common.h"
#include "pc_error_codes.h"
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h> /* getenv, llabs, rand, srand, system */
#include <string.h>
#include <stdio.h>
#include <time.h>

static void need_args(PcErrorCtx *err, size_t argc, size_t need, PcSourceLoc loc) {
  if (argc < need) pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "not enough arguments to built-in function");
}

static int64_t to_int(const PcValue *v) {
  if (!v->type) return 0;
  if (v->type->kind == PC_T_INT) return (int64_t)v->i;
  if (v->type->kind == PC_T_REAL) return (int64_t)v->r;
  return 0;
}

static double to_real(const PcValue *v) {
  if (!v->type) return 0;
  if (v->type->kind == PC_T_REAL) return v->r;
  if (v->type->kind == PC_T_INT) return (double)v->i;
  return 0;
}

bool pc_stdlib_try_call(PcErrorCtx *err, const char *name, PcValue *argv, size_t argc, PcValue *out,
                        PcSourceLoc loc) {
  char *n = pc_strdup(name);
  if (!n) return false;
  pc_str_tolower(n);
  bool ok = true;

  if (strcmp(n, "length") == 0) {
    need_args(err, argc, 1, loc);
    if (argv[0].type && argv[0].type->kind == PC_T_STRING) {
      size_t L = argv[0].s ? strlen(argv[0].s) : 0;
      pc_value_init_int(out, (int64_t)L);
    } else if (argv[0].type && argv[0].type->kind == PC_T_ARRAY) {
      int c = (argv[0].type->hi0 - argv[0].type->lo0 + 1);
      if (argv[0].type->is2d) c *= (argv[0].type->hi1 - argv[0].type->lo1 + 1);
      pc_value_init_int(out, c);
    } else
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "LENGTH expects STRING or ARRAY");
    free(n);
    return true;
  }

  if (strcmp(n, "substring") == 0 || strcmp(n, "mid") == 0) {
    need_args(err, argc, 3, loc);
    if (!argv[0].type || argv[0].type->kind != PC_T_STRING || !argv[0].s) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "first argument must be STRING");
      free(n);
      return true;
    }
    int start = (int)to_int(&argv[1]);
    int len = (int)to_int(&argv[2]);
    const char *s = argv[0].s;
    size_t sl = strlen(s);
    if (start < 1) start = 1;
    size_t i0 = (size_t)start - 1;
    if (i0 > sl) i0 = sl;
    if (len < 0) len = 0;
    size_t take = (size_t)len;
    if (i0 + take > sl) take = sl - i0;
    char *buf = malloc(take + 1);
    if (!buf) {
      pc_runtime_error(err, loc, PC_ERR_RUN_OOM, "out of memory");
      free(n);
      return true;
    }
    memcpy(buf, s + i0, take);
    buf[take] = '\0';
    pc_value_init_string(out, buf, true);
    free(n);
    return true;
  }

  if (strcmp(n, "right") == 0) {
    need_args(err, argc, 2, loc);
    if (!argv[0].type || argv[0].type->kind != PC_T_STRING || !argv[0].s) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "RIGHT expects STRING");
      free(n);
      return true;
    }
    int x = (int)to_int(&argv[1]);
    const char *s = argv[0].s;
    size_t sl = strlen(s);
    if (x < 0) x = 0;
    size_t take = (size_t)x;
    if (take > sl) take = sl;
    char *buf = malloc(take + 1);
    if (!buf) {
      pc_runtime_error(err, loc, PC_ERR_RUN_OOM, "out of memory");
      free(n);
      return true;
    }
    memcpy(buf, s + (sl - take), take);
    buf[take] = '\0';
    pc_value_init_string(out, buf, true);
    free(n);
    return true;
  }

  if (strcmp(n, "concat") == 0) {
    need_args(err, argc, 2, loc);
    const char *a = (argv[0].s) ? argv[0].s : "";
    const char *b = (argv[1].s) ? argv[1].s : "";
    size_t la = strlen(a), lb = strlen(b);
    char *buf = malloc(la + lb + 1);
    if (!buf) {
      pc_runtime_error(err, loc, PC_ERR_RUN_OOM, "out of memory");
      free(n);
      return true;
    }
    memcpy(buf, a, la);
    memcpy(buf + la, b, lb + 1);
    pc_value_init_string(out, buf, true);
    free(n);
    return true;
  }

  if (strcmp(n, "tointeger") == 0 || strcmp(n, "int") == 0) {
    need_args(err, argc, 1, loc);
    if (!argv[0].type) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_CONV, "invalid value for TOINTEGER");
      free(n);
      return true;
    }
    if (argv[0].type->kind == PC_T_STRING && argv[0].s)
      pc_value_init_int(out, (int64_t)strtoll(argv[0].s, NULL, 10));
    else
      pc_value_init_int(out, to_int(&argv[0]));
    free(n);
    return true;
  }

  if (strcmp(n, "toreal") == 0) {
    need_args(err, argc, 1, loc);
    if (argv[0].type->kind == PC_T_STRING && argv[0].s)
      pc_value_init_real(out, strtod(argv[0].s, NULL));
    else
      pc_value_init_real(out, to_real(&argv[0]));
    free(n);
    return true;
  }

  if (strcmp(n, "random") == 0) {
    if (argc == 0) {
      pc_value_init_real(out, (double)rand() / ((double)RAND_MAX + 1.0));
      free(n);
      return true;
    }
    need_args(err, argc, 1, loc);
    double lim = (double)to_int(&argv[0]);
    if (lim <= 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_RANGE, "RANDOM/RAND upper bound must be positive");
      free(n);
      return true;
    }
    pc_value_init_real(out, ((double)rand() / ((double)RAND_MAX + 1.0)) * lim);
    free(n);
    return true;
  }

  if (strcmp(n, "rand") == 0) {
    free(n);
    return pc_stdlib_try_call(err, "random", argv, argc, out, loc);
  }

  if (strcmp(n, "round") == 0) {
    need_args(err, argc, 1, loc);
    double r = to_real(&argv[0]);
    pc_value_init_int(out, (int64_t)floor(r + 0.5));
    free(n);
    return true;
  }

#define M1(name, fn)                                                                               \
  if (strcmp(n, name) == 0) {                                                                      \
    need_args(err, argc, 1, loc);                                                                  \
    pc_value_init_real(out, fn(to_real(&argv[0])));                                                \
    free(n);                                                                                       \
    return true;                                                                                   \
  }
  M1("sqrt", sqrt)
  M1("sin", sin)
  M1("cos", cos)
  M1("tan", tan)
  M1("asin", asin)
  M1("acos", acos)
  M1("atan", atan)
  M1("log", log)
  M1("log10", log10)
  M1("exp", exp)
  M1("floor", floor)
  M1("ceil", ceil)
#undef M1

  if (strcmp(n, "atan2") == 0) {
    need_args(err, argc, 2, loc);
    pc_value_init_real(out, atan2(to_real(&argv[0]), to_real(&argv[1])));
    free(n);
    return true;
  }
  if (strcmp(n, "hypot") == 0) {
    need_args(err, argc, 2, loc);
    pc_value_init_real(out, hypot(to_real(&argv[0]), to_real(&argv[1])));
    free(n);
    return true;
  }
  if (strcmp(n, "pow") == 0) {
    need_args(err, argc, 2, loc);
    pc_value_init_real(out, pow(to_real(&argv[0]), to_real(&argv[1])));
    free(n);
    return true;
  }
  if (strcmp(n, "fmod") == 0 || strcmp(n, "modreal") == 0) {
    need_args(err, argc, 2, loc);
    pc_value_init_real(out, fmod(to_real(&argv[0]), to_real(&argv[1])));
    free(n);
    return true;
  }
  if (strcmp(n, "abs") == 0) {
    need_args(err, argc, 1, loc);
    if (argv[0].type && argv[0].type->kind == PC_T_INT)
      pc_value_init_int(out, (int64_t)llabs(argv[0].i));
    else
      pc_value_init_real(out, fabs(to_real(&argv[0])));
    free(n);
    return true;
  }
  if (strcmp(n, "pi") == 0) {
    if (argc != 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "PI takes no arguments");
      free(n);
      return true;
    }
    pc_value_init_real(out, 3.14159265358979323846);
    free(n);
    return true;
  }
  if (strcmp(n, "euler") == 0 || strcmp(n, "e_const") == 0) {
    if (argc != 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "EULER takes no arguments");
      free(n);
      return true;
    }
    pc_value_init_real(out, 2.71828182845904523536);
    free(n);
    return true;
  }
  if (strcmp(n, "radians") == 0) {
    need_args(err, argc, 1, loc);
    pc_value_init_real(out, to_real(&argv[0]) * (3.14159265358979323846 / 180.0));
    free(n);
    return true;
  }
  if (strcmp(n, "degrees") == 0) {
    need_args(err, argc, 1, loc);
    pc_value_init_real(out, to_real(&argv[0]) * (180.0 / 3.14159265358979323846));
    free(n);
    return true;
  }
  if (strcmp(n, "trunc") == 0) {
    need_args(err, argc, 1, loc);
    pc_value_init_real(out, trunc(to_real(&argv[0])));
    free(n);
    return true;
  }

  if (strcmp(n, "time") == 0) {
    if (argc != 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "TIME takes no arguments");
      free(n);
      return true;
    }
    pc_value_init_real(out, (double)time(NULL));
    free(n);
    return true;
  }
  if (strcmp(n, "clockms") == 0) {
    if (argc != 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "CLOCKMS takes no arguments");
      free(n);
      return true;
    }
    clock_t c = clock();
    if (c == (clock_t)-1) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "CLOCKMS unavailable");
      free(n);
      return true;
    }
    pc_value_init_int(out, (int64_t)((double)c * 1000.0 / (double)CLOCKS_PER_SEC));
    free(n);
    return true;
  }
  if (strcmp(n, "nowstring") == 0) {
    if (argc != 0) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "NOWSTRING takes no arguments");
      free(n);
      return true;
    }
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char buf[64];
    if (!tm || !strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", tm)) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "NOWSTRING format failed");
      free(n);
      return true;
    }
    pc_value_init_string(out, pc_strdup(buf), true);
    free(n);
    return true;
  }

  if (strcmp(n, "getenv") == 0) {
    need_args(err, argc, 1, loc);
    if (!argv[0].type || argv[0].type->kind != PC_T_STRING || !argv[0].s) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "GETENV expects STRING name");
      free(n);
      return true;
    }
    const char *v = getenv(argv[0].s);
    pc_value_init_string(out, pc_strdup(v ? v : ""), true);
    free(n);
    return true;
  }
  if (strcmp(n, "system") == 0) {
    need_args(err, argc, 1, loc);
    if (!argv[0].type || argv[0].type->kind != PC_T_STRING || !argv[0].s) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_ARG, "SYSTEM expects STRING shell command");
      free(n);
      return true;
    }
    int r = system(argv[0].s);
    pc_value_init_int(out, (int64_t)r);
    free(n);
    return true;
  }

  if (strcmp(n, "randomseed") == 0 || strcmp(n, "seedrandom") == 0 || strcmp(n, "seed") == 0) {
    need_args(err, argc, 1, loc);
    srand((unsigned)(uint32_t)to_int(&argv[0]));
    pc_value_init_int(out, 0);
    free(n);
    return true;
  }
  if (strcmp(n, "randint") == 0 || strcmp(n, "randomint") == 0) {
    need_args(err, argc, 2, loc);
    int64_t lo = to_int(&argv[0]);
    int64_t hi = to_int(&argv[1]);
    if (hi < lo) {
      pc_runtime_error(err, loc, PC_ERR_BUILTIN_RANGE, "RANDINT: upper < lower");
      free(n);
      return true;
    }
    uint64_t span = (uint64_t)(hi - lo + 1);
    uint64_t r = (uint64_t)((double)rand() / ((double)RAND_MAX + 1.0) * (double)span);
    pc_value_init_int(out, lo + (int64_t)r);
    free(n);
    return true;
  }

  if (strcmp(n, "uppercase") == 0) {
    need_args(err, argc, 1, loc);
    if (!argv[0].s) {
      pc_value_init_string(out, pc_strdup(""), true);
      free(n);
      return true;
    }
    char *buf = pc_strdup(argv[0].s);
    for (char *p = buf; *p; p++) *p = (char)toupper((unsigned char)*p);
    pc_value_init_string(out, buf, true);
    free(n);
    return true;
  }

  if (strcmp(n, "lowercase") == 0) {
    need_args(err, argc, 1, loc);
    if (!argv[0].s) {
      pc_value_init_string(out, pc_strdup(""), true);
      free(n);
      return true;
    }
    char *buf = pc_strdup(argv[0].s);
    for (char *p = buf; *p; p++) *p = (char)tolower((unsigned char)*p);
    pc_value_init_string(out, buf, true);
    free(n);
    return true;
  }

  if (strcmp(n, "ucase") == 0) {
    need_args(err, argc, 1, loc);
    char c = argv[0].c;
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    pc_value_init_char(out, c);
    free(n);
    return true;
  }

  if (strcmp(n, "lcase") == 0) {
    need_args(err, argc, 1, loc);
    char c = argv[0].c;
    if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a');
    pc_value_init_char(out, c);
    free(n);
    return true;
  }

  (void)ok;
  free(n);
  return false;
}
