#include "i8042.h"


.global _asm_handler

.text
_asm_handler:
	movl	OUT_BUFF, %eax

	ret
