#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned long rb;

	rb=TIMER_RB_CMD;
	rb= rb|TIMER_RB_COUNT_;
	rb= rb|TIMER_RB_SEL(timer);

	sys_outb(TIMER_CTRL,rb);
	sys_inb(timer,st);
	return 1;
}

int timer_display_conf(unsigned char conf) {
	printf("Output pin state: ")
	if (conf & BIT_7 == BIT_7) {
		printf("1");
	}
	else {
		printf("0");
	}

	printf("Null count flags: ")
	if (conf & BIT_6 == BIT_6) {
		printf("1");
	}
	else {
		printf("0");
	}

	printf("Acess mode: ");
	if (conf & BIT_5 == BIT_5) {
		if (conf & BIT_4 == BIT_4)
			printf("1 1 - Acess mode: lobyte/hibyte");
		else
			printf("1 0 - Acess mode: hibyte only");
	}
	else{
		if (conf & BIT_4 == BIT_4)
			printf("0 1 - Acess mode: lobyte only");
		else
			printf("0 0 - Latch count value command");
	}

	print("Operanting mode: ");
	if (conf & BIT_3 == BIT_3) {
		if (conf & BIT_2 == BIT_2) {
			if (conf & BIT_1 == BIT_1) {
				printf("Mode 3: square wave generator");
			}
			else {
				printf("Mode 2: rate generator");
			}
		}
		else {
			if (conf & BIT_1 == BIT_1) {
				printf("Mode 5: hardware triggered strobe");
			}
			else {
				printf("Mode 4: software triggered strobe");
			}
		}
	}
	else {
		if (conf & BIT_2 == BIT_2) {
			if (conf & BIT_1 == BIT_1) {
				printf("Mode 3: square wave generator");
			}
			else {
				printf("Mode 2: rate generator");
			}
		}
		else {
			if (conf & BIT_1 == BIT_1) {
				printf("Mode 1: hardware re-triggered one-shot");
			}
			else {
				printf("Mode 0: interrupt on terminal count");
			}
		}
	}

	printf("BCD/Binary mode: ")
	if (conf & BIT_0 == BIT_0){
		printf("16-bit binary");
	}
	else {
		printf("four-digit BCD");
	}


	return 1;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;
	timer_get_conf(timer,&conf);
	timer_display_conf();
	return 1;
}
