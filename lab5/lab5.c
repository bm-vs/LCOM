#include <minix/drivers.h>
#include <stdlib.h>

#include "constants.h"
#include "test5.h"

static int proc_args(int argc, char *argv[]);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {


  /* Initialize service */

  sef_startup();


  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {
      proc_args(argc, argv);
  }
  return 0;

}

static void print_usage(char *argv[]) {
  printf("\nUsage: one of the following:\n"
	 "\t service run %s -args \"init <unsigned short mode> <unsigned short delay>\" \n"
	 "\t service run %s -args \"square <unsigned short x> <unsigned short y> <unsigned short size> "
		  "<unsigned long color>\" \n"
     "\t service run %s -args \"line <unsigned short xi> <unsigned short yi> <unsigned short xf>"
		  "<unsigned short yf> <unsigned long color>\" \n"
	 "\t service run %s -args \"xpm <unsigned short xi> <unsigned short yi> <char *xpm[]>\" \n"
	 "\t service run %s -args \"move <unsigned short xi> <unsigned short yi> <char *xpm[]> <unsigned short hor>"
		  "<short delta> <unsigned short time>\" \n"
	 "\t service run %s -args \"controller\" \n",
	 argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if(argc != 4) {
			printf("init: wrong no of arguments for test of test_init() \n");
			return 1;
		}

		unsigned short mode;
		unsigned short delay;

		printf("\ntest_init()\n");

		mode = strtoul(argv[2], NULL, HEX);
		delay = strtoul(argv[3], NULL, DEC);

		test_init(mode, delay);
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if(argc != 6) {
			printf("square: wrong no of arguments for test of test_square() \n");
			return 1;
		}

		unsigned short x;
		unsigned short y;
		unsigned short size;
		unsigned long color;

		printf("\ntest_square()\n");
		x = strtoul(argv[2], NULL, DEC);
		y = strtoul(argv[3], NULL, DEC);
		size = strtoul(argv[4], NULL, DEC);
		color = strtoul(argv[5], NULL, DEC);

		test_square(x, y, size, color);
	}
	else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if(argc != 7) {
			printf("line: wrong no of arguments for test of test_line() \n");
			return 1;
		}

		unsigned short xi;
		unsigned short yi;
		unsigned short xf;
		unsigned short yf;
		unsigned long color;

		printf("\ntest_line()\n");
		xi = strtoul(argv[2], NULL, DEC);
		yi = strtoul(argv[3], NULL, DEC);
		xf = strtoul(argv[4], NULL, DEC);
		yf = strtoul(argv[5], NULL, DEC);
		color = strtoul(argv[6], NULL, DEC);

		test_line(xi, yi, xf, yf, color);
	}
 }
