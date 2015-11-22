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

	return sp;
}


int draw_sprite(Sprite *sp, char *video_mem) {
	return vg_draw_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

void erase_sprite(Sprite *sp, char *video_mem) {
	vg_erase_pixmap(sp->x, sp->y, sp->height, sp->width, sp->map, video_mem);
}

void move_sprite(Sprite *sp, short x, short y, unsigned short h_res, unsigned short v_res) {
	if ((sp->x + x + sp->width < h_res) && (sp->y + y + sp->height < v_res) &&
			(sp->x + x >= 0) && (sp->y + y >= 0)) {
		sp->x += x;
		sp->y += y;
	}
}


void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;
}
