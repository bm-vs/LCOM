#include <stddef.h>
#include "sprite.h"
#include "video_gr.h"

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

	return sp;
}


int draw_sprite(Sprite *sp, char *video_mem) {
	return vg_draw_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

void erase_sprite(Sprite *sp, char *video_mem) {
	vg_erase_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

void move_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem) {
	if (check_collision_sprite(sp, h_res, v_res, video_mem) == 0) {
		sp->x += sp->x_speed;
		sp->y += sp->y_speed;
	}
}

int check_collision_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem) {
	// With down border
	if (sp->y + sp->y_speed + sp->height >= v_res) {
		return 1;
	}
	// With other borders
	else if ((sp->x + sp->x_speed + sp->width >= h_res)
			|| (sp->y + sp->y_speed < 0) ||
				(sp->x + sp->x_speed < 0)) {
		return 2;
	}

	// With other sprites

	// Not colliding
	else {
		return 0;
	}
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
