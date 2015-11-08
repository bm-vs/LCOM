#include <minix/drivers.h>
#include <stdlib.h>

#include "test4.h"
#include "mouse.h"
#include "constants.h"

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
	 "\t service run %s -args \"packet <unsigned short cnt>\" \n"
	 "\t service run %s -args \"async <unsigned short idle_time>\" \n"
     "\t service run %s -args \"config\" \n",
	 "\t service run %s -args \"gesture <short length> <unsigned short tolerance>\" \n",
	 argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	unsigned short cnt;
	unsigned short idle_time;
	short length;
	unsigned short tolerance;

	if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
		if(argc != 3) {
			printf("packet: wrong no of arguments for test of test_packet() \n");
			return 1;
		}

		printf("\ntest_packet()\n");
		cnt = atoi(argv[2]);

		test_packet(cnt);
	}
	else if (strncmp(argv[1], "async", strlen("async")) == 0) {
		if(argc != 3) {
			printf("async: wrong no of arguments for test of test_async() \n");
			return 1;
		}

		printf("\ntest_async()\n");
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
	}
 }
