#ifndef PC_ERROR_H
#define PC_ERROR_H

#include <stdbool.h>
#include <stdio.h>

typedef struct PcSourceLoc {
  const char *filename;
  int line;
  int column;
} PcSourceLoc;

typedef struct PcErrorCtx {
  bool had_error;
  bool trace;
  bool color;
  FILE *err;
} PcErrorCtx;

void pc_err_init(PcErrorCtx *ctx, bool trace, bool color);
void pc_error_at(PcErrorCtx *ctx, PcSourceLoc loc, const char *fmt, ...);
void pc_runtime_error(PcErrorCtx *ctx, PcSourceLoc loc, const char *fmt, ...);
void pc_note(PcErrorCtx *ctx, const char *fmt, ...);

#endif
