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
	if (x < 0 || x >= H_RES) {
		printf("Invalid value of x.\n");
		return 1;
	}
	else if (y < 0 || y >= V_RES) {
		printf("Invalid value of y.\n");
		return 1;
	}
	else if (size < 0) {
		printf("Invalid value of size.\n");
		return 1;
	}
	else if ((x + size) > H_RES || (y + size) > V_RES) {
		printf("Invalid size for given position.\n");
		return 1;
	}

	
	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);

	vg_draw_square(x, y, size, color, video_mem);

	if (kbd_scan_esc() == 1) {
		printf("Error on keyboard read.\n");
	}

	vg_exit();
	
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {
	
	if (xi < 0 || xi >= H_RES) {
		printf("Invalid value of xi.\n");
		return 1;
	}
	else if (yi < 0 || yi >= V_RES) {
		printf("Invalid value of yi.\n");
		return 1;
	}
	if (xf < 0 || xf >= H_RES) {
		printf("Invalid value of xf.\n");
		return 1;
	}
	else if (yf < 0 || yf >= V_RES) {
		printf("Invalid value of yf.\n");
		return 1;
	}



	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);

	vg_draw_line(xi, yi, xf, yf, color, video_mem);

	exit:
		if (kbd_scan_esc() == 1) {
			printf("Error on keyboard read.\n");
		}

		vg_exit();

		return 0;
	
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
	
