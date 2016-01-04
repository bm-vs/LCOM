#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <math.h>
#include "vbe.h"
#include "video_gr.h"

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned mr_size;

void *vg_init(unsigned short mode) {
	struct reg86u reg86;
	struct mem_range mr;
	vbe_mode_info_t vbe_mode_info;

	reg86.u.b.intno = VBE_INTERRUPT;
	reg86.u.b.ah = VBE_FUNCTION;
	reg86.u.b.al = SET_VIDEO_MODE;
	reg86.u.w.bx = (mode | BIT(LINEAR_MODEL_BIT));

	sys_int86(&reg86);

	vbe_get_mode_info(mode, &vbe_mode_info);

	h_res = vbe_mode_info.XResolution;
	v_res = vbe_mode_info.YResolution;
	bits_per_pixel = vbe_mode_info.BitsPerPixel;
	mr_size = h_res * v_res * bits_per_pixel;


	mr.mr_base = vbe_mode_info.PhysBasePtr;

	mr.mr_limit = mr.mr_base + mr_size/8;

	if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
		return NULL;
	}

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, mr_size/8);

	return video_mem;
}


int vg_draw_pixmap(unsigned short xi, unsigned short yi, unsigned short height, unsigned short width,
		char *pixmap, char *video_mem) {

	// When image does not fit the screen
	if (xi + width >= H_RES || yi + height >= V_RES) {
		return 1;
	}

	unsigned long vram_pos;
	unsigned long sprite_pos = 0;
	vram_pos = yi * H_RES + xi;

	int y_pos;
	int x_pos;
	for (y_pos = 0; y_pos < height; y_pos++) {
		for (x_pos = 0; x_pos < width; x_pos++) {
			if (pixmap[sprite_pos] != 0) {
				video_mem[vram_pos] = pixmap[sprite_pos];
			}
			vram_pos++;
			sprite_pos++;
		}

		vram_pos = (yi + y_pos) * H_RES + xi;
	}

	return 0;
}


int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = VBE_INTERRUPT;
	reg86.u.b.ah = SET_TEXT_FUNCTION;
	reg86.u.b.al = TEXT_MODE;

	if( sys_int86(&reg86) != OK ) {
	  printf("\tvg_exit(): sys_int86() failed \n");
	  return 1;
	} else
	  return 0;
}
