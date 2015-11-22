#ifndef __SPRITE_H
#define __SPRITE_H


typedef struct {
	int x, y; //current position
	int width, height; // dimensions
	char *map; // the pixmap
} Sprite;

Sprite *create_sprite(char *pic[], unsigned short x, unsigned short y);
int draw_sprite(Sprite *sp, char *video_mem);
void erase_sprite(Sprite *sp, char *video_mem);
void move_sprite(Sprite *sp, short x, short y, unsigned short h_res, unsigned short v_res);
void destroy_sprite(Sprite *sp);


#endif
