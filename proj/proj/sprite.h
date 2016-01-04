#ifndef __SPRITE_H
#define __SPRITE_H



typedef struct {
	int x, y; 
	int width , height;
	int x_speed, y_speed;
	int jumping;
	int inplatform;
	char *map;
} Sprite;



Sprite *create_sprite(char *pic[], unsigned short x, unsigned short y);

int draw_sprite(Sprite *sp, char *video_mem);

int move_sprite(Sprite *sp, unsigned short h_res, unsigned short v_res, char *video_mem);

int move_shot(Sprite *sp, Sprite *p, unsigned short h_res, unsigned short v_res, char *video_mem);

void set_x_speed_sprite(Sprite *sp, int x_speed);

void set_y_speed_sprite(Sprite *sp, int y_speed);

void destroy_sprite(Sprite *sp);


#endif
