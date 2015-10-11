#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <math.h>
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char cmd;
	unsigned long new_freq = TIMER_FREQ/freq;
	unsigned char lsb = new_freq;
	unsigned char msb = new_freq >> 8;

	switch(timer) {
	case 0:
		cmd = TIMER_SEL0;
		break;
	case 1:
		cmd = TIMER_SEL1;
		break;
	case 2:
		cmd = TIMER_SEL2;
		break;
	}

	cmd = cmd | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN;

	sys_outb(TIMER_CTRL, cmd);
	sys_outb(TIMER_0 + timer, lsb);
	sys_outb(TIMER_0 + timer, msb);

	return 0;
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
	unsigned long timer_info;

	rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);

	sys_outb(TIMER_CTRL,rb);
	sys_inb(TIMER_0 + timer, &timer_info);

	*st = timer_info;

	return 0;
}

int timer_display_conf(unsigned char conf) {
	printf("\n");

	int i;
	for (i = 0; i < 8; i++) {
		if (conf & BIT(7-i)) {
			printf("1");
		}
		else {
			printf("0");
		}
	}

	printf("\n\n");

	printf("   Output pin state: ");
	if ((conf & BIT_7) == BIT_7) {
		printf("1\n");
	}
	else {
		printf("0\n");
	}

	printf("   Null count flags: ");
	if ((conf & BIT_6) == BIT_6) {
		printf("1\n");
	}
	else {
		printf("0\n");
	}

	printf("   Acess mode: ");
	if ((conf & BIT_5) == BIT_5) {
		if ((conf & BIT_4) == BIT_4)
			printf("1 1 - LSB followed by MSB\n");
		else
			printf("1 0 - MSB\n");
	}
	else{
		if ((conf & BIT_4) == BIT_4)
			printf("0 1 - LSB\n");
		else
			printf("0 0 - Latch count value command\n");
	}

	printf("   Operanting mode: ");
	if ((conf & BIT_3) == BIT_3) {
		if ((conf & BIT_2) == BIT_2) {
			if ((conf & BIT_1) == BIT_1) {
				printf("Mode 3 - square wave generator\n");
			}
			else {
				printf("Mode 2 - rate generator\n");
			}
		}
		else {
			if ((conf & BIT_1) == BIT_1) {
				printf("Mode 5 - hardware triggered strobe\n");
			}
			else {
				printf("Mode 4 - software triggered strobe\n");
			}
		}
	}
	else {
		if ((conf & BIT_2) == BIT_2) {
			if ((conf & BIT_1) == BIT_1) {
				printf("Mode 3 - square wave generator\n");
			}
			else {
				printf("Mode 2 - rate generator\n");
			}
		}
		else {
			if ((conf & BIT_1) == BIT_1) {
				printf("Mode 1 - hardware re-triggered one-shot\n");
			}
			else {
				printf("Mode 0 - interrupt on terminal count\n");
			}
		}
	}

	printf("   BCD/Binary mode: ");
	if ((conf & BIT_0) == BIT_0){
		printf("four-digit BCD\n");
	}
	else {
		printf("16-bit binary\n");
	}


	return 0;
}

int timer_test_square(unsigned long freq) {
	
	timer_set_square(0, freq);

	return 0;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;

	timer_get_conf(timer,&conf);
	timer_display_conf(conf);
	return 1;
}
