#ifndef PC_PARSER_H
#define PC_PARSER_H

#include "pc_ast.h"
#include "pc_lexer.h"
#include "pc_error.h"

PcAst *pc_parse_program(PcLexer *L, PcErrorCtx *err);

#endif
