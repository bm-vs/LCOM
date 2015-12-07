#ifndef __SPRITE_H
#define __SPRITE_H


typedef struct {
	int x, y; //current position
	int width, height; // dimensions
	int x_speed, y_speed; // speed
	int jumping;
	char *map; // the pixmap
} Sprite;

Sprite *create_sprite(char *pic[], unsigned short x, unsigned short y);
int draw_sprite(Sprite *sp, char *video_mem);
void erase_sprite(Sprite *sp, char *video_mem);
void move_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem);
int check_collision_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem);
void set_x_speed_sprite(Sprite *sp, int x_speed);
void set_y_speed_sprite(Sprite *sp, int y_speed);
void destroy_sprite(Sprite *sp);


#endif
