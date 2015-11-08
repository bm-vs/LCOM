#include "mouse.h"

int mouse_hook_id = 0;

int mouse_subscribe_int() {
	mouse_hook_id = MOUSE_HOOK_BIT;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) == OK) {
		if (sys_irqenable(&mouse_hook_id) == OK) {
			return MOUSE_HOOK_BIT;
		}
	}

	return -1;
}

int mouse_unsubscribe_int() {
	if (sys_irqrmpolicy(&mouse_hook_id) == OK) {
		if (sys_irqdisable(&mouse_hook_id) == OK) {
			return 0;
		}
	}

	return 1;
}

int mouse_set_stream_mode() {
	unsigned long stat = 0;
	int cmd_receive = 0;
	unsigned long data = 0;

	while(1) {
		while(1) {
			sys_inb(STAT_REG, &stat);
			if ((stat & IBF) == 0) {
				if (sys_outb(STAT_REG, WRITE_TO_MOUSE) == OK) {
					break;
				}
			}
		}

		while(1) {
			sys_inb(STAT_REG, &stat);
			if ((stat & IBF) == 0) {
				if (sys_outb(OUT_BUFF, SET_STREAM_MODE) == OK) {
					break;
				}
			}
		}

		while(1) {
			sys_inb(STAT_REG, &stat);
			if (stat & OBF) {
				sys_inb(OUT_BUFF, &data);
				cmd_receive = data;
				break;
			}
		}

		break;
	}
}

int mouse_enable_stream() {
	unsigned long stat = 0;

	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(STAT_REG, WRITE_TO_MOUSE) == OK) {
				break;
			}
		}
	}

	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, DATA_STREAM_ON) == OK) {
				break;
			}
		}
	}
}

int print_packet(char data_packet[]) {
	printf("B1=0x%02x ", (data_packet[0]));
	printf("B2=0x%02x ", (data_packet[1]));
	printf("B3=0x%02x ", (data_packet[2]));

	if (LB == (data_packet[0] & LB)) {
		printf("LB=1 ");
	} else {
		printf("LB=0 ");
	}

	if (MB == (data_packet[0] & MB)) {
		printf("MB=1 ");
	} else {
		printf("MB=0 ");
	}

	if (RB == (data_packet[0] & RB)) {
		printf("RB=1 ");
	} else {
		printf("RB=0 ");
	}

	if (XOV == (data_packet[0] & XOV)) {
		printf("XOV=1 ");
	} else {
		printf("XOV=0 ");
	}

	if (YOV == (data_packet[0] & YOV)) {
		printf("YOV=1 ");
	} else {
		printf("YOV=0 ");
	}

	if (XSIGN == (data_packet[0] & XSIGN)) {
		printf("X=%d ", data_packet[1]-255);
	} else {
		printf("X=%d ", data_packet[1]);
	}

	if (YSIGN == (data_packet[0] & YSIGN)) {
		printf("Y=%d ", data_packet[2]-255);
	} else {
		printf("Y=%d ", data_packet[2]);
	}
	printf("\n");
}
