#ifndef PC_ERROR_H
#define PC_ERROR_H

#include <stdbool.h>
#include <stdio.h>

typedef struct PcSourceLoc {
  const char *filename;
  int line;
  int column;
} PcSourceLoc;

typedef enum PcErrKind {
  PC_ERRK_NONE = 0,
  PC_ERRK_PARSE,
  PC_ERRK_RUNTIME,
  PC_ERRK_IO,
} PcErrKind;

typedef struct PcErrorCtx {
  bool had_error;
  PcErrKind kind;
  int last_code; /* last [PC-NNN] emitted */
  bool trace;
  bool color;
  bool quiet; /* when true, errors are not printed (e.g. CASE arm lookahead) */
  FILE *err;
} PcErrorCtx;

void pc_err_init(PcErrorCtx *ctx, bool trace, bool color);
void pc_err_clear(PcErrorCtx *ctx); /* reset had_error/kind for REPL continuation */

void pc_error_at(PcErrorCtx *ctx, PcSourceLoc loc, int code, const char *fmt, ...);
void pc_runtime_error(PcErrorCtx *ctx, PcSourceLoc loc, int code, const char *fmt, ...);
void pc_io_error(PcErrorCtx *ctx, const char *path, int code, const char *fmt, ...);
void pc_note(PcErrorCtx *ctx, const char *fmt, ...);

#endif
