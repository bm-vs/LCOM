#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

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


void vg_draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color, char *video_mem) {
	unsigned long pos;
	pos = y * H_RES + x;

	int y_pos;
	int x_pos;
	for (y_pos = 0; y_pos < size; y_pos++) {
		for (x_pos = 0; x_pos < size; x_pos++) {
			video_mem[pos] = color;
			pos++;
		}
		pos = (y + y_pos) * H_RES + x;
	}
}


void vg_draw_line(unsigned short xi, unsigned short yi,
		           unsigned short xf, unsigned short yf, unsigned long color, char *video_mem) {


	unsigned long pos;
	float add = 0;
	float slope;

	// Swap values so that the initial point
	// is always to the left of the final one
	if (xf < xi) {
		swap(&xi, &xf);
		swap(&yi, &yf);
	}

	// Vertical lines
	if (xi == xf) {
		if (yf < yi) {
			swap(&yi, &yf);
		}

		while (yi <= yf) {
			pos = yi * H_RES + xi;

			video_mem[pos] = color;
			yi++;
		}
	}
	// Every other line
	else {
		slope = (yf - yi)/((float) xf - xi);

		// From top to bottom
		while ((xi != xf) || (yi != yf)) {
			pos = yi * H_RES + xi;

			video_mem[pos] = color;

			if (abs(slope) == 1) {
				xi++;
				yi += slope;
			}
			else if (slope < 1 && slope > -1) {
				add += slope;
				if (xi != xf) {
					xi++;
				}
				if (add >= 1) {
					yi++;
					add--;
				}
				else if (add <= -1) {
					yi--;
					add++;
				}
			}
			else if (slope > 1 || slope < -1) {
				add += 1/slope;
				if (yi != yf && slope > 1) {
					yi++;
				}
				else if (yi != yf && slope < -1) {
					yi--;
				}

				if (add > 1) {
					xi++;
					add--;
				}
				else if (add < -1) {
					xi++;
					add++;
				}
			}

		}
	}
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
