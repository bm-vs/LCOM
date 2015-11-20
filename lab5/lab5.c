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
	unsigned short mode;
	unsigned short delay;

	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if(argc != 4) {
			printf("init: wrong no of arguments for test of test_init() \n");
			return 1;
		}

		printf("\ntest_init()\n");

		mode = strtoul(argv[2], NULL, HEX);
		delay = strtoul(argv[3], NULL, DEC);

		test_init(mode, delay);
	} /*
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if(argc != 5) {
			printf("square: wrong no of arguments for test of test_square() \n");
			return 1;
		}

		printf("\ntest_square()\n");
		idle_time = atoi(argv[2]);

		test_async(idle_time);
	}
	else if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if(argc != 2) {
			printf("config: wrong no of arguments for test of test_config() \n");
			return 1;
		}

		printf("\ntest_config()\n");
		test_config();
	}
	else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
		if(argc != 4) {
			printf("gesture: wrong no of arguments for test of test_gesture() \n");
			return 1;
		}

		printf("\ntest_gesture()\n");
		length = atoi(argv[2]);
		tolerance = atoi(argv[3]);

		test_gesture(length, tolerance);
	}*/
 }
