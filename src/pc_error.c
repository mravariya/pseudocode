#include "pc_error.h"
#include <stdarg.h>
#include <stdlib.h>

static const char *c_red = "";
static const char *c_yellow = "";
static const char *c_dim = "";
static const char *c_reset = "";
static const char *c_magenta = "";

#ifdef _WIN32
#include <io.h>
#define ISATTY_F _isatty(_fileno(stderr))
#else
#include <unistd.h>
#define ISATTY_F isatty(STDERR_FILENO)
#endif

void pc_err_init(PcErrorCtx *ctx, bool trace, bool color) {
  ctx->had_error = false;
  ctx->kind = PC_ERRK_NONE;
  ctx->last_code = 0;
  ctx->trace = trace;
  ctx->color = color && ISATTY_F;
  ctx->quiet = false;
  ctx->err = stderr;
  if (ctx->color) {
    c_red = "\x1b[31m";
    c_yellow = "\x1b[33m";
    c_dim = "\x1b[90m";
    c_magenta = "\x1b[35m";
    c_reset = "\x1b[0m";
  } else {
    c_red = c_yellow = c_dim = c_magenta = c_reset = "";
  }
}

void pc_err_clear(PcErrorCtx *ctx) {
  ctx->had_error = false;
  ctx->kind = PC_ERRK_NONE;
  ctx->last_code = 0;
  ctx->quiet = false;
}

static const char *kind_label(PcErrKind k) {
  switch (k) {
  case PC_ERRK_PARSE:
    return "parse";
  case PC_ERRK_RUNTIME:
    return "runtime";
  case PC_ERRK_IO:
    return "io";
  default:
    return "error";
  }
}

static void emit(PcErrorCtx *ctx, PcErrKind kind, PcSourceLoc loc, int code, const char *kind_word,
                 const char *fmt, va_list ap) {
  ctx->had_error = true;
  ctx->kind = kind;
  ctx->last_code = code;
  if (ctx->quiet) return;
  fprintf(ctx->err, "%s%s%s", c_red, kind_word, c_reset);
  fprintf(ctx->err, " %s[PC-%d]%s", c_magenta, code, c_reset);
  fprintf(ctx->err, " [%s]", kind_label(kind));
  if (loc.filename)
    fprintf(ctx->err, " %s%s:%d:%d%s", c_dim, loc.filename, loc.line, loc.column, c_reset);
  fprintf(ctx->err, ": ");
  vfprintf(ctx->err, fmt, ap);
  fputc('\n', ctx->err);
}

void pc_error_at(PcErrorCtx *ctx, PcSourceLoc loc, int code, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  emit(ctx, PC_ERRK_PARSE, loc, code, "error", fmt, ap);
  va_end(ap);
}

void pc_runtime_error(PcErrorCtx *ctx, PcSourceLoc loc, int code, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  emit(ctx, PC_ERRK_RUNTIME, loc, code, "error", fmt, ap);
  va_end(ap);
}

void pc_io_error(PcErrorCtx *ctx, const char *path, int code, const char *fmt, ...) {
  PcSourceLoc loc = {path, 0, 0};
  va_list ap;
  va_start(ap, fmt);
  emit(ctx, PC_ERRK_IO, loc, code, "error", fmt, ap);
  va_end(ap);
}

void pc_note(PcErrorCtx *ctx, const char *fmt, ...) {
  fprintf(ctx->err, "%snote:%s ", c_dim, c_reset);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(ctx->err, fmt, ap);
  va_end(ap);
  fputc('\n', ctx->err);
}
