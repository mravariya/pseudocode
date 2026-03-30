#ifndef PC_PKG_SHARED_H
#define PC_PKG_SHARED_H

#include "pc_error.h"

int pc_pkg_install_local(const char *src_dir, const char *pkg_name, PcErrorCtx *err);
int pc_pkg_list(void);
int pc_pkg_remove(const char *name);
void pc_pkg_print_registry_note(void);

#endif
