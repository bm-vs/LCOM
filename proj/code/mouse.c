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
			return 0;
	}

	return 1;
}

int mouse_set_stream_mode() {
	unsigned long stat = 0;
	int cmd_receive = 0;
	unsigned long data = 0;

	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(STAT_REG, WRITE_TO_MOUSE) == OK) {
					break;
			}

			return 1;
		}
	}


	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(STAT_REG, SET_STREAM_MODE) == OK) {
				return 0;
			}

			return 1;
		}
	}


}

int mouse_enable_stream() {
	unsigned long stat = 0;
	int cmd_receive = 0;
	unsigned long data = 0;

	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(STAT_REG, WRITE_TO_MOUSE) == OK) {
					break;
			}

			return 1;
		}
	}


	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, DATA_STREAM_ON) == OK) {
				return 0;
			}

			return 1;
		}
	}
}

int mouse_status_request() {
	unsigned long stat = 0;
	int cmd_receive = 0;
	unsigned long data = 0;

	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(STAT_REG, WRITE_TO_MOUSE) == OK) {
					break;
			}

			return 1;
		}
	}


	while(1) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, STATUS_REQUEST) == OK) {
				return 0;
			}

			return 1;
		}
	}
}

long get_byte() {
	long stat = 0;
	long data;

	while (1) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			sys_inb(OUT_BUFF, &data);
			return data;
		}
		sleep(2);
	}
}



int packet_handler(char data_packet[], Sprite *sprite, char *video_mem) {
	// MOVE LEFT
	if (XSIGN == (data_packet[0] & XSIGN)) {
		if (sprite->x + (short) (0xff00 |  data_packet[1]) >= 0) {
			sprite->x += (short) (0xff00 |  data_packet[1]);
		}
	// MOVE RIGHT
	} else {
		if (sprite->x + sprite->width + data_packet[1] < H_RES) {
			sprite->x += data_packet[1];
		}
	}

	// MOVE DOWN
	if (YSIGN == (data_packet[0] & YSIGN)) {
		if (sprite->y + sprite->height - (short) (0xff00 | data_packet[2]) < V_RES) {
			sprite->y -= (short) (0xff00 | data_packet[2]);
		}
	// MOVE UP
	} else {
		if (sprite->y - data_packet[2] >= 0) {
			sprite->y -= data_packet[2];
		}
	}

	// IF LEFT BUTTON IS PRESSED SHOOT
	if (LB == (data_packet[0] & LB)) {
		return 1;
	}

	return 0;
}
