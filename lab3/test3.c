#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <math.h>
#include "i8254.h"
#include "i8042.h"
#include "kbd.h"

int kbd_test_scan(unsigned short ass) {
	if (ass == 0) {

		if (kbd_subscribe_int() != 0) {
			return 1;
		}

		int r, ipc_status, scan_result;
		message msg;

		while(1) {
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
		    }

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
		                if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {
		                	if (kbd_int_handler() == 1) {
		                		delay(20000);
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
}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
