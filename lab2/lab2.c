#include <minix/drivers.h>
#include <stdlib.h>
#include "i8254.h"


static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
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
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"square <unsigned long freq>\" \n"
	 "\t service run %s -args \"int <unsigned long time>\" \n"
     "\t service run %s -args \"config <unsigned long timer>\" \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	unsigned long timer;
	unsigned long freq;
	unsigned long time;

  /* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "config", strlen("config")) == 0) {
		if( argc != 3) {
			printf("timer_test_config: wrong no of arguments for test of timer_test_config() \n");
			return 1;
		}

		printf("\ntimer_test_config()\n"); /* Actually, it was already invoked */
		timer = atol(argv[2]);

		timer_test_config(timer);
	}
	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 3) {
			printf("timer_test_square: wrong no of arguments for test of timer_test_square() \n");
			return 1;
		}

		printf("\ntimer_test_square()\n");
		freq = atol(argv[2]);

		timer_test_square(freq);
	}
	else if (strncmp(argv[1], "int", strlen("int")) == 0) {
			if (argc != 3) {
				printf("timer_test_int: wrong no of arguments for test of timer_test_int() \n");
				return 1;
			}

			printf("\ntimer_test_int()\n");
			time = atol(argv[2]);

			timer_test_int(time);
		}
 }

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
