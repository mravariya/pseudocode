#ifndef PC_ERROR_CODES_H
#define PC_ERROR_CODES_H

/* Numeric codes appear in messages as [PC-NNN] for tooling and docs. */
enum {
  PC_ERR_PARSE_EXPECT = 101,   /* expected a specific token */
  PC_ERR_PARSE_TYPE = 102,     /* type / declaration syntax */
  PC_ERR_PARSE_EXPR = 103,     /* expression syntax */
  PC_ERR_PARSE_STMT = 104,     /* statement syntax */
  PC_ERR_PARSE_PARAM = 105,    /* procedure/function parameter */
  PC_ERR_PARSE_EOF = 110,      /* trailing garbage / expected EOF */

  PC_ERR_RUN_UNDEF = 201,      /* undefined variable */
  PC_ERR_RUN_INDEX = 202,      /* array index / shape */
  PC_ERR_RUN_CALL = 203,       /* undefined function / bad call */
  PC_ERR_RUN_RETURN = 204,     /* missing RETURN */
  PC_ERR_RUN_TYPE = 205,       /* type mismatch for operation */
  PC_ERR_RUN_REDECL = 206,     /* duplicate declaration */
  PC_ERR_RUN_CONTROL = 207,    /* FOR / CASE / BYREF issues */
  PC_ERR_RUN_FILE = 208,       /* file open / I/O in program */
  PC_ERR_RUN_DIV = 220,        /* division or MOD by zero */
  PC_ERR_RUN_OOM = 230,        /* allocation failed in interpreter path */

  PC_ERR_BUILTIN_ARG = 251,    /* built-in: wrong args / type */
  PC_ERR_BUILTIN_CONV = 252,   /* built-in: conversion failed */
  PC_ERR_BUILTIN_RANGE = 253,  /* built-in: range error */

  PC_ERR_IO_FILE = 301,        /* cannot read/write host file (CLI, pkg) */
};

#endif
