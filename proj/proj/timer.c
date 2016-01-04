#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "timer.h"

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