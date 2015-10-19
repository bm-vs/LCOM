#include <minix/drivers.h>
#include <stdlib.h>

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
	 "\t service run %s -args \"scan <unsigned short ass>\" \n"
	 "\t service run %s -args \"leds <unsigned short n> <unsigned short *leds>\" \n"
     "\t service run %s -args \"timed_scan <unsigned short n>\" \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	unsigned short ass;
	unsigned short n;
	unsigned short *leds;

  /* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if( argc != 3) {
			printf("kbd_test_scan: wrong no of arguments for test of kbd_test_scan() \n");
			return 1;
		}

		printf("\nkbd_test_scan()\n"); /* Actually, it was already invoked */
		ass = atoi(argv[2]);

		kbd_test_scan(ass);
	}
	else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
		if (argc != 3) {
			printf("kbd_test_leds: wrong no of arguments for test of kbd_test_leds() \n");
			return 1;
		}

		printf("\nkbd_test_leds()\n");
		n = atoi(argv[2]);
		leds = argv[3];

		kbd_test_leds(n, leds);
	}
	else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
			if (argc != 3) {
				printf("kbd_test_timed_scan: wrong no of arguments for test of kbd_test_timed_scan() \n");
				return 1;
			}

			printf("\nkbd_test_timed_scan()\n");
			n = atoi(argv[2]);

			kbd_test_timed_scan(n);
		}
 }
