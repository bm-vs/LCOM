#include "test5.h"

void *test_init(unsigned short mode, unsigned short delay) {

	char* video_mem;
	video_mem = vg_init(mode);

	timer_test_int(delay);

	vg_exit();

	printf("\nPhysical address of VRAM: 0x%X\n", video_mem);
	return video_mem;
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	
	/* To be completed */
	
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	/* To be completed */
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	
	/* To be completed */
	
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	/* To be completed */
	
}					

int test_controller() {
	
	/* To be completed */
	
}					
	
