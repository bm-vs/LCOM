#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/sysutil.h>
#include <minix/com.h>

#include "vbe.h"
#include "lmlib.h"

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg86;
	mmap_t map;

	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t), &map);

	reg86.u.b.intno = VBE_INTERRUPT;
	reg86.u.b.ah = VBE_FUNCTION;
	reg86.u.b.al = RETURN_MODE_INFO;
	reg86.u.w.cx = (mode | BIT(LINEAR_MODEL_BIT));
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	sys_int86(&reg86);

	*vmi_p = *(vbe_mode_info_t*)map.virtual;
	lm_free(&map);

	return 0;
}