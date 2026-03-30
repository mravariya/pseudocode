#ifndef PC_INTERP_H
#define PC_INTERP_H

#include "pc_ast.h"
#include "pc_error.h"

typedef struct Interp Interp;

Interp *pc_interp_new(PcErrorCtx *err);
void pc_interp_free(Interp *I);

bool pc_interp_run(Interp *I, PcAst *program);
bool pc_interp_check(Interp *I, PcAst *program);
void pc_interp_set_trace(Interp *I, bool on);

#endif
