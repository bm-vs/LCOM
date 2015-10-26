#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <math.h>
#include "i8254.h"
#include "i8042.h"
#include "timer.h"
#include "kbd.h"

int kbd_test_scan(unsigned short ass) {
	if (ass == 0) {

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
						if (kbd_int_handler() == 1) {
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
	}

	return 1;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {

	message msg;
	unsigned long int info;
	unsigned long cmd;

	int scroll = 0, num = 0, caps = 0;

	if (timer_subscribe_int() != 0) {
		return 1;
	}
	if (timer_set_square(0, 60) != 0) {
		return 1;
	}

	int i;
	for (i = 0; i < n; i++) {

		sys_outb(OUT_BUFF, TOGGLE_LEDS);
		sys_inb(OUT_BUFF, &info);

		timer_test_int(1);

		if (leds[i] == 0) { // scroll lock
			if (scroll == 0) {
				scroll = SCROLL_LOCK;
				printf("scroll on\n");
			} else {
				scroll = 0;
				printf("scroll off\n");
			}
		} else if (leds[i] == 1) { // num lock
			if (num == 0) {
				num = NUM_LOCK;
				printf("num on\n");
			} else {
				num = 0;
				printf("scroll off\n");
			}
		} else if (leds[i] == 2) { // caps lock
			if (caps == 0) {
				caps = CAPS_LOCK;
				printf("caps on\n");
			} else {
				caps = 0;
				printf("caps off\n");
			}
		}

		cmd = scroll | num | caps;
		sys_outb(OUT_BUFF, cmd);
		sys_inb(OUT_BUFF, &info);

	}

	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	if (kbd_subscribe_int() != 1) {
		return 1;
	}
	if (timer_subscribe_int() != 0) {
		return 1;
	}

	int r, ipc_status, scan_result;
	unsigned char timer_hook_bit = 0;
	message msg;
	int counter = 0;

	while (counter < n * TIMER_DEFAULT_FREQ) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {
					counter = 0;
					if (kbd_int_handler() == 1) {
						kbd_unsubscribe_int();
						return 0;
					}
				}
				if (msg.NOTIFY_ARG & BIT(timer_hook_bit)) {
					counter++;
				}

				break;
			default:
				break;
			}
		}

	}

	kbd_unsubscribe_int();
	timer_unsubscribe_int();

	return 0;
}
