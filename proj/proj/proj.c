#include <minix/drivers.h>
#include <stdlib.h>

#include "constants.h"
#include "test.h"
#include "pixmap.h"

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
	 "\t service run %s -args \"move\" \n",
	 argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	if (strncmp(argv[1], "ms", strlen("ms")) == 0) {
		if(argc != 2) {
			printf("ms: wrong no of arguments \n");
			return 1;
		}

		move_and_shoot();
	}
	if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if(argc != 2) {
			printf("move: wrong no of arguments \n");
			return 1;
		}

		move();
	}
	if (strncmp(argv[1], "shoot", strlen("shoot")) == 0) {
		if(argc != 2) {
			printf("shoot: wrong no of arguments \n");
			return 1;
		}

		shoot();
	}
	if (strncmp(argv[1], "vector", strlen("vector")) == 0) {
		if(argc != 2) {
			printf("vector: wrong no of arguments \n");
			return 1;
		}

		test_vector();
	}
 }
