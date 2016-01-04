#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "sprite.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions related to graphics card.
 */


/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Draws a pixmap on the given buffer.
 * 
 * @param xi coordinate of the pixmap.
 * @param yi coordinate of the pixmap.
 * @param height height of the pixmap.
 * @param width width of the pixmap.
 * @param pixmap pointer to the pixmap to draw.
 * @param video_mem pointer to the buffer to draw in.
 * @return 0 if the sprites fits in the buffer. 1 if not.
 */
int vg_draw_pixmap(unsigned short xi, unsigned short yi, unsigned short height, unsigned short width,
		char *pixmap, char *video_mem);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
