#include "pc_error.h"
#include <stdarg.h>
#include <stdlib.h>

static const char *c_red = "";
static const char *c_yellow = "";
static const char *c_dim = "";
static const char *c_reset = "";

#ifdef _WIN32
#include <io.h>
#define ISATTY_F _isatty(_fileno(stderr))
#else
#include <unistd.h>
#define ISATTY_F isatty(STDERR_FILENO)
#endif

void pc_err_init(PcErrorCtx *ctx, bool trace, bool color) {
  ctx->had_error = false;
  ctx->trace = trace;
  ctx->color = color && ISATTY_F;
  ctx->err = stderr;
  if (ctx->color) {
    c_red = "\x1b[31m";
    c_yellow = "\x1b[33m";
    c_dim = "\x1b[90m";
    c_reset = "\x1b[0m";
  } else {
    c_red = c_yellow = c_dim = c_reset = "";
  }
}

void pc_error_at(PcErrorCtx *ctx, PcSourceLoc loc, const char *fmt, ...) {
  ctx->had_error = true;
  fprintf(ctx->err, "%serror%s", c_red, c_reset);
  if (loc.filename)
    fprintf(ctx->err, " at %s:%d:%d", loc.filename, loc.line, loc.column);
  fprintf(ctx->err, ": ");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(ctx->err, fmt, ap);
  va_end(ap);
  fputc('\n', ctx->err);
}

void pc_runtime_error(PcErrorCtx *ctx, PcSourceLoc loc, const char *fmt, ...) {
  ctx->had_error = true;
  fprintf(ctx->err, "%sruntime error%s", c_red, c_reset);
  if (loc.filename)
    fprintf(ctx->err, " at %s:%d:%d", loc.filename, loc.line, loc.column);
  fprintf(ctx->err, ": ");
  va_list ap;
  va_start(ap, fmt);
  vfprintf(ctx->err, fmt, ap);
  va_end(ap);
  fputc('\n', ctx->err);
}

void pc_note(PcErrorCtx *ctx, const char *fmt, ...) {
  fprintf(ctx->err, "%snote:%s ", c_dim, c_reset);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(ctx->err, fmt, ap);
  va_end(ap);
  fputc('\n', ctx->err);
}
