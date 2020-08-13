#ifndef _ASM_UML_UNWIND_H
#define _ASM_UML_UNWIND_H

#ifdef CONFIG_UML_X86
#include "../../../x86/include/asm/unwind.h"
#else
static inline void unwind_init(void) {}
static inline void
unwind_module_init(struct module *mod, void *orc_ip, size_t orc_ip_size,
		   void *orc, size_t orc_size) {}
#endif

#endif /* _ASM_UML_UNWIND_H */
