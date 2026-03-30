#ifndef PC_STDLIB_H
#define PC_STDLIB_H

#include "pc_error.h"
#include "pc_value.h"
#include <stddef.h>

bool pc_stdlib_try_call(PcErrorCtx *err, const char *name, PcValue *argv, size_t argc, PcValue *out,
                        PcSourceLoc loc);

#endif
