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
			video_mem[vram_pos] = pixmap[sprite_pos];
			vram_pos++;
			sprite_pos++;
		}

		vram_pos = (yi + y_pos) * H_RES + xi;
	}

	return 0;
}


void vg_erase_pixmap(unsigned short xi, unsigned short yi, unsigned short height, unsigned short width,
		char *pixmap, char *video_mem) {

	unsigned long vram_pos;
	unsigned long sprite_pos = 0;
	vram_pos = yi * H_RES + xi;

	int y_pos;
	int x_pos;
	for (y_pos = 0; y_pos < height; y_pos++) {
		for (x_pos = 0; x_pos < width; x_pos++) {
			video_mem[vram_pos] = BLACK;
			vram_pos++;
			sprite_pos++;
		}

		vram_pos = (yi + y_pos) * H_RES + xi;
	}
}


int vg_move_sprite(Sprite *sprite, unsigned short hor, short delta, unsigned short time, char *video_mem) {

	int ipc_status, r;
	message msg;
	int counter = 0;
	int timer_hook_bit = 0;

	if (timer_set_square(0, 60) != 0) {
		return 1;
	}

	if (timer_subscribe_int() != 0) {
		return 1;
	}

	draw_sprite(sprite, video_mem);

	double move_time;

	// Divides the total time of the movement by the distance moved
	// move_time is the amount of time between the sprite moving

	move_time = (float) time*TIMER_DEFAULT_FREQ / abs(delta);

	// Used to track the time of the next interrupt
	double next_int_time = move_time;

	// move_distance is the number of pixels the sprite moves every interrupt
	// move_error is used to track the error of this movement
	double move_distance;
	double move_error = modf(1/move_time, &move_distance);
	double move_total_error = 0;

	// if move_distance == 0 the sprite moves 1 pixel every interrupt
	if (move_distance == 0) {
		move_distance = 1;
		move_error = 0;
	}

	// if the move time == 0 it means that every interrupt it has to move more than 1 pixel
	if (move_time < 1) {
		move_time = 1;
	}

	while(counter <= time*TIMER_DEFAULT_FREQ) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
					if (msg.NOTIFY_ARG & BIT(timer_hook_bit)) {

						if (counter == (int)floor(next_int_time)) {

							next_int_time += move_time;

							// Erase sprite from current position
							erase_sprite(sprite, video_mem);

							// Move sprite
							if (move_total_error >= 1) {
								move_distance--;
								move_total_error--;
							}

							move_total_error += move_error;

							// Move it an extra pixel to compensate for the error
							if (move_total_error >= 1) {
								move_distance++;
							}


							if (hor == 0) {
								if (delta > 0) {
									move_sprite(sprite, move_distance, 0, H_RES, V_RES);
								}
								else {
									move_sprite(sprite, -move_distance, 0, H_RES, V_RES);
								}
							}
							else {
								if (delta > 0) {
									move_sprite(sprite, 0, move_distance, H_RES, V_RES);
								}
								else {
									move_sprite(sprite, 0, -move_distance, H_RES, V_RES);
								}
							}

							// Draw sprite in new position
							if (draw_sprite(sprite, video_mem) == 1) {
								break;
							}
						}

						counter++;
					}
					break;
				default:
					break;
			}
		} else {
			continue;
		}
	 }

	destroy_sprite(sprite);
	if (timer_unsubscribe_int() != 0) {
		return 1;
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
