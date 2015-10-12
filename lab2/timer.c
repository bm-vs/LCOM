#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <math.h>
#include "i8254.h"

int counter = 0;
int timer_irq = 0;
int timer_hook_bit = 0;
int hook_id = 0;

int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned char cmd;
	unsigned long new_freq = TIMER_FREQ/freq;
	unsigned char lsb = new_freq;
	unsigned char msb = new_freq >> 8;

	if (timer_get_conf(timer, &cmd) != 0) {
		return 1;
	}

	switch(timer) {
	case 0:
		cmd |= TIMER_SEL0;
		break;
	case 1:
		cmd |= TIMER_SEL1;
		break;
	case 2:
		cmd |= TIMER_SEL2;
		break;
	}


	cmd = cmd | TIMER_LSB_MSB | TIMER_SQR_WAVE;

	if (sys_outb(TIMER_CTRL, cmd) == OK) {
		if (sys_outb(TIMER_0 + timer, lsb) == OK) {
			if (sys_outb(TIMER_0 + timer, msb) == OK) {
				return 0;
			}
		}
	}

	return 1;
}

int timer_subscribe_int() {
	hook_id = timer_hook_bit;

	if(sys_irqsetpolicy(timer_irq, IRQ_REENABLE , &hook_id) == OK) {
		if (sys_irqenable(&hook_id) == OK) {
			return timer_hook_bit;
		}
	}

	return -1;
}

int timer_unsubscribe_int() {

	if (sys_irqrmpolicy(&hook_id) == OK) {
		return 0;
	}

	return 1;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned long rb;
	unsigned long timer_info;

	rb = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);

	if (sys_outb(TIMER_CTRL,rb) == OK) {
		if (sys_inb(TIMER_0 + timer, &timer_info) == OK) {
			*st = timer_info;

			return 0;
		}
	}

	return 1;
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
	if (freq == 0) {
		printf("Invalid frequency\n");
		return 2;
	}
	
	if (timer_set_square(0, freq) == 0) {
		return 0;
	}

	return 1;
}

int timer_test_int(unsigned long time) {
	int ipc_status, r;
	message msg;

	if (timer_set_square(0, 60) != 0) {
		return 1;
	}

	if (timer_subscribe_int() != 0) {
		return 1;
	}

	printf("\n");

	while(counter <= time*TIMER_DEFAULT_FREQ) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
	    }

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
	                if (msg.NOTIFY_ARG & BIT(timer_hook_bit)) {
	                	timer_int_handler();

	                	if (counter % TIMER_DEFAULT_FREQ == 0) {
	                		if (counter == SECOND) {
	                			printf("1 second\n");
	                		}
	                		else {
	                			printf("%d seconds\n", counter/SECOND);
	                		}
	                	}
	                }
	                break;
	            default:
	                break;
	        }
	    } else {
	    	continue;
	    }
	 }

	if (timer_unsubscribe_int() != 0) {
		return 1;
	}
	
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;

	if (timer > 2 || timer < 0) {
		printf("Invalid timer\n");
		return 2;
	}


	if (timer_get_conf(timer,&conf) == 0) {
		timer_display_conf(conf);

		return 0;
	}

	return 1;
}
