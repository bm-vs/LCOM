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
		vg_exit();
		printf("Error on keyboard read.\n");
		return 1;
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
			vg_exit();
			printf("Error on keyboard read.\n");
			return 1;
		}

		vg_exit();

		return 0;
	
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	if (xi < 0 || xi >= H_RES) {
		printf("Invalid value of xi.\n");
		return 1;
	}
	else if (yi < 0 || yi >= V_RES) {
		printf("Invalid value of yi.\n");
		return 1;
	}

	
	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);

	int width, height;
	char *pixmap = read_xpm(xpm, &width, &height);

	if (vg_draw_pixmap(xi, yi, width, height, pixmap, video_mem) == 1) {
		vg_exit();
		printf("Image does not fit the screen at those coordinates.\n");
		return 1;
	}


	if (kbd_scan_esc() == 1) {
		vg_exit();
		printf("Error on keyboard read.\n");
		return 1;
	}
	
	vg_exit();

	return 0;
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time) {
	
	if (xi < 0 || xi >= H_RES) {
		printf("Invalid value of xi.\n");
		return 1;
	}
	else if (yi < 0 || yi >= V_RES) {
		printf("Invalid value of yi.\n");
		return 1;
	}


	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);

	Sprite *sprite = create_sprite(xpm, xi, yi);
	vg_move_sprite(sprite, hor, delta, time, video_mem);

	if (kbd_scan_esc() == 1) {
		vg_exit();
		printf("Error on keyboard read.\n");
		return 1;
	}
	
	vg_exit();

	return 0;

}					

int test_controller() {
	
	void* init = lm_init();

	uint16_t *video_modes;
	vbe_info_block vbe_info;
	unsigned num_video_modes;
	mmap_t map;

	lm_alloc(sizeof(vbe_info_block), &map);

	memcpy(vbe_info.VbeSignature, VBE_SIGNATURE, sizeof(VBE_SIGNATURE));

	struct reg86u reg86;

	reg86.u.b.intno = VBE_INTERRUPT;
	reg86.u.b.ah = VBE_FUNCTION;
	reg86.u.b.al = RETURN_DATA;
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	sys_int86(&reg86);

	vbe_info = *((vbe_info_block *)map.virtual);

	void *ptr = (void *)((vbe_info.VideoModePtr & 0xffff0000) >> 12);
	ptr += PB2OFF(vbe_info.VideoModePtr);
	ptr += (uint32_t)map.virtual & 0xF0000000;

	lm_free(&map);
	
	int16_t *modes = ptr;

	num_video_modes = 0;

	while (*modes != -1) {
		modes++;
		num_video_modes++;
	}

	video_modes = malloc(num_video_modes * sizeof(uint16_t));

	int i;
	modes = ptr;
	for (i = 0; i < num_video_modes; i++) {
		video_modes[i] = modes;
		modes++;
	}

	printf("\n\nCapabilities: 0x%02x\n\n", vbe_info.Capabilities);
	if (vbe_info.Capabilities & D0 == D0) {
		printf("DAC width is switchable to 8 bits per primary color\n");
	} else {
		printf("DAC is fixed width, with 6 bits per primary color\n");
	}

	if (vbe_info.Capabilities & D1 == D1) {
		printf("Controller is not VGA compatible\n");
	} else {
		printf("Controller is VGA compatible\n");
	}

	if (vbe_info.Capabilities & D2 == D2) {
		printf("When programming large blocks of information to the RAMDAC,\nuse the blank bit in Function 09h.\n");
	} else {
		printf("Normal RAMDAC operation\n");
	}

	printf("\nVideo modes:\n");

	for (i = 0; i < num_video_modes; i++) {
		printf("0x%02x", video_modes[i]);
		if (i != num_video_modes - 1) {
			printf(", ");
		}
	}

	free(video_modes);
	printf("\n\nSize of VRAM: %lu KB\n", vbe_info.TotalMemory * 64);

	return 0;
}					
	
