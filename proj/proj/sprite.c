#include <stddef.h>
#include "sprite.h"
#include "video_gr.h"
#include "xpm.h"
#include "constants.h"

Sprite *create_sprite(char *pic[], unsigned short x, unsigned short y) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;

	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}

	sp->x = x;
	sp->y = y;

	sp->x_speed = 0;
	sp->y_speed = 0;

	sp->jumping = 0;
	sp->inplatform = 0;

	return sp;
}


int draw_sprite(Sprite *sp, char *video_mem) {
	return vg_draw_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

void erase_sprite(Sprite *sp, char *video_mem) {
	vg_erase_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

int move_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem) {
	// Check if sprite can be moved

	// BORDERS

	// Collision with down border
	if (sp->y + sp->y_speed + sp->height >= v_res) {
		sp->jumping = 0;
		set_y_speed_sprite(sp, 0);
		sp->y = v_res - sp->height - 1;
		return 1;
	}
	// Collision with upper border
	if (sp->y + sp->y_speed < 0) {
		// do nothing
		return 2;
	}
	// Collision with right border
	else if (sp->x + sp->x_speed + sp->width >= h_res) {
		set_x_speed_sprite(sp, 0);
		sp->x = h_res - sp->width - 1;
		return 3;
	}
	// Collision with left border
	else if (sp->x + sp->x_speed < 0) {
		set_x_speed_sprite(sp, 0);
		sp->x = 0;
		return 4;
	}


	// OTHER SPRITES
	unsigned long vram_pos;
	int y_pos;
	int x_pos;

	// Top border
	y_pos = 0;
	vram_pos = sp->y * H_RES + sp->x;
	for (x_pos = 0; x_pos < sp->width; x_pos++) {
		// if any pixel the sprite is supposed to be in is already colored
		int vram_next_pos = (sp->y + y_pos + sp->y_speed) * H_RES + (sp->x + x_pos);
		if (video_mem[vram_next_pos] != BLACK) {
			sp->y_speed = 0;
			return 5;
		}

		vram_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos);
	}

	// Bottom border
	if (sp->y_speed != 0) {
		y_pos = sp->height - 1;
		vram_pos = sp->y * H_RES + sp->x;
		for (x_pos = 0; x_pos < sp->width; x_pos++) {
			// if any pixel the sprite is supposed to be in is already colored
			int vram_next_pos = (sp->y + y_pos + sp->y_speed) * H_RES + (sp->x + x_pos);
			if (video_mem[vram_next_pos] != BLACK) {
				int next = 0;
				while (video_mem[vram_pos] == BLACK) {
					vram_pos = (sp->y + y_pos + next) * H_RES + (sp->x + x_pos);
					next++;
				}

				sp->y += next - 1;

				sp->y_speed = 0;
				sp->jumping = 0;
				sp->inplatform = 1;
				return 6;
			}

			vram_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos);
		}
	}


	// Left border
	if (sp->x_speed != 0) {
		x_pos = 0;
		vram_pos = sp->y * H_RES + sp->x;
		for (y_pos = 0; y_pos < sp->height - 1; y_pos++) {
			// if any pixel the sprite is supposed to be in is already colored
			int vram_next_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos + sp->x_speed);
			if (video_mem[vram_next_pos] != BLACK) {
				sp->x_speed = 0;
				sp->y += sp->y_speed;
				return 7;
			}

			vram_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos);
		}
	}

	// Right border
	if (sp->x_speed != 0) {
		x_pos = sp->width - 1;
		vram_pos = sp->y * H_RES + sp->x;
		for (y_pos = 0; y_pos < sp->height - 1; y_pos++) {
			// if any pixel the sprite is supposed to be in is already colored
			int vram_next_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos + sp->x_speed);
			if (video_mem[vram_next_pos] != BLACK) {
				sp->x_speed = 0;
				sp->y += sp->y_speed;
				return 7;
			}

			vram_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos);
		}
	}

	// Check if sprite ran off of platform
	int true = 0;
	if (sp->inplatform == 1) {
		y_pos = sp->height - 1;
		for (x_pos = 0; x_pos < sp->width; x_pos++) {
			// if any pixel the sprite is supposed to be in is already colored
			int vram_next_pos = (sp->y + y_pos + 10) * H_RES + (sp->x + x_pos);
			if (video_mem[vram_next_pos] == BLACK) {
				true = 1;
			}
			else {
				true = 0;
				break;
			}
		}
	}

	if (true == 1) {
		sp->jumping = 1;
		sp->inplatform = 0;
	}


	// Normal movement
	sp->x += sp->x_speed;
	sp->y += sp->y_speed;
	return 0;
}

int move_shot(Sprite *sp, Sprite *p, unsigned short h_res, unsigned short v_res, char *video_mem) {
	// Check if sprite can be moved

	// BORDERS

	// Collision with down border
	if (sp->y + sp->y_speed + sp->height >= v_res) {
		return 1;
	}
	// Collision with upper border
	if (sp->y + sp->y_speed < 0) {
		return 2;
	}
	// Collision with right border
	else if (sp->x + sp->x_speed + sp->width >= h_res) {
		return 3;
	}
	// Collision with left border
	else if (sp->x + sp->x_speed < 0) {
		return 4;
	}


	// OTHER SPRITES
	int y_pos;
	int x_pos;

	int vram_pos = sp->y * H_RES + sp->x;
	for (x_pos = 0; x_pos < sp->width; x_pos++) {
		for (y_pos = 0; y_pos < sp->width; y_pos++) {
			// if any pixel the sprite is supposed to be in is already colored
			int vram_next_pos = (sp->y + y_pos + sp->y_speed) * H_RES + (sp->x + x_pos + sp->x_speed);
			if (video_mem[vram_next_pos] != BLACK) {
				// Check if its a player
				if ((sp->y + y_pos >= p->y) && (sp->y + y_pos <= p->y + p->height - 1)) {
					if ((sp->x + x_pos >= p->x) && (sp->x + x_pos <= p->x + p->width - 1)) {
						return 6;
					}
				}
				// Check if its a platform
				if (video_mem[vram_next_pos] == PLATFORM_COLOR_1 || video_mem[vram_next_pos] == PLATFORM_COLOR_2) {
					return 5;
				}

			}


			vram_pos = (sp->y + y_pos) * H_RES + (sp->x + x_pos);
		}
	}


	// Normal movement
	sp->x += sp->x_speed;
	sp->y += sp->y_speed;
	return 0;
}


void set_x_speed_sprite(Sprite *sp, int x_speed) {
	sp->x_speed = x_speed;
}

void set_y_speed_sprite(Sprite *sp, int y_speed) {
	sp->y_speed = y_speed;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
}
