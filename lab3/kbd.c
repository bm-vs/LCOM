#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "i8254.h"
#include "i8042.h"
#include "kbd.h"

int kbd_hook_id = 0;

int kbd_subscribe_int() {
	kbd_hook_id = KBD_HOOK_BIT;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)
			== OK) {
		if (sys_irqenable(&kbd_hook_id) == OK) {
			return KBD_HOOK_BIT;
		}
	}

	return -1;
}

int kbd_unsubscribe_int() {
	if (sys_irqrmpolicy(&kbd_hook_id) == OK) {
		if (sys_irqdisable(&kbd_hook_id) == OK) {
			return 0;
		}
	}

	return 1;
}

int kbd_int_handler() {
	unsigned long stat = 0;

	sys_inb(OUT_BUFF, &stat);

	if ((stat & BREAKCODE) == BREAKCODE) {
		printf("Break code: 0x%02x\n\n", stat);
	} else {
		printf("Make code: 0x%02x\n", stat);
	}

	if (stat == ESC) {
		return 1;
	}

	return 0;
}

