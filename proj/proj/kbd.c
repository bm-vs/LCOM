#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constants.h"
#include "kbd.h"

int kbd_hook_id = 1;

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

	if ((stat & BREAKCODE) != BREAKCODE) {
		return stat;
	}
}

int kbd_scan_esc() {

	if (kbd_subscribe_int() != 1) {
		return 1;
	}

	int r, ipc_status, scan_result;
	message msg;

	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {
					unsigned long stat = 0;
					sys_inb(OUT_BUFF, &stat);

					if (stat == ESC) {
						kbd_unsubscribe_int();
						return 0;
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

	kbd_unsubscribe_int();

	return 1;
}
