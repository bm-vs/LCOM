#include "pixmap.h"

char **get_pixmap(unsigned short xpm) {
	char **images[N_IMAGES];

	images[0] = pic1;
	images[1] = pic2;
	images[2] = cross;
	images[3] = pic3;
	images[4] = penguin;

	if (xpm < 0 || xpm >= N_IMAGES) {
		return NULL;
	}
	else {
		return images[xpm];
	}
}
