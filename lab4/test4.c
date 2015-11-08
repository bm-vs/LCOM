#include "test4.h"

int test_packet(unsigned short cnt){
	message msg;
	int ipc_status, r;
	long data;
	int n_packets = 0;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];

	if (mouse_subscribe_int() == -1) {
		return 1;
	}

	// Set stream mode
	mouse_set_stream_mode();
	mouse_enable_stream();

	while (n_packets < cnt) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & MOUSE_HOOK_BIT) {
					byte_read = sys_inb(OUT_BUFF, &data);

					switch(byte_nr) {
						case 1: {
							if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
								byte1 = byte_read;
								byte_nr = 2;
							}
						}
						case 2: {
							byte2 = byte_read;
							byte_nr = 3;
						}
						case 3: {
							byte3 = byte_read;
							byte_nr = 1;

							data_packet[0] = byte1;
							data_packet[1] = byte2;
							data_packet[2] = byte3;

							n_packets++;
							print_packet(data_packet);
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

	mouse_unsubscribe_int();
}

int test_async(unsigned short idle_time) {
	message msg;
		int ipc_status, r;
		long data;
		int byte_nr = 1;
		int counter = 0;
		int tmp_counter = 0;
		char byte_read, byte1, byte2, byte3;
		char data_packet[3];

		if (mouse_subscribe_int() == -1) {
			return 1;
		}
		if (timer_subscribe_int() == -1) {
			return 1;
		}


		mouse_set_stream_mode();
		mouse_enable_stream();


		while (counter < idle_time) {
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & TIMER_HOOK_BIT) {
						tmp_counter++;
						if (tmp_counter % 60 == 0) {
							tmp_counter = 0;
							counter++;
						}
					}
					if (msg.NOTIFY_ARG & MOUSE_HOOK_BIT) {
						byte_read = sys_inb(OUT_BUFF, &data);

						switch(byte_nr) {
							case 1: {
								if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
									byte1 = byte_read;
									byte_nr = 2;
								}
							}
							case 2: {
								byte2 = byte_read;
								byte_nr = 3;
							}
							case 3: {
								byte3 = byte_read;
								byte_nr = 1;

								data_packet[0] = byte1;
								data_packet[1] = byte2;
								data_packet[2] = byte3;

								print_packet(data_packet);
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

		timer_unsubscribe_int();
		mouse_unsubscribe_int();
}

int test_config(void) {
    /* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
