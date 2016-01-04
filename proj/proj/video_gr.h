#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "sprite.h"


void *vg_init(unsigned short mode);

int vg_draw_pixmap(unsigned short xi, unsigned short yi, unsigned short height, unsigned short width,
		char *pixmap, char *video_mem);

int vg_exit(void);

 
#endif /* __VIDEO_GR_H */
