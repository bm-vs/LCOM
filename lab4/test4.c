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
				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {

					byte_read = get_byte();

					switch(byte_nr) {
						case 1: {
							if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
								byte1 = byte_read;
								byte_nr = 2;
							}
							break;
						}
						case 2: {
							byte2 = byte_read;
							byte_nr = 3;
							break;
						}
						case 3: {
							byte3 = byte_read;
							byte_nr = 1;

							data_packet[0] = byte1;
							data_packet[1] = byte2;
							data_packet[2] = byte3;

							n_packets++;
							print_packet(data_packet);
							break;
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
				if (msg.NOTIFY_ARG & BIT(TIMER_HOOK_BIT)) {
					tmp_counter++;
					if (tmp_counter % 60 == 0) {
						tmp_counter = 0;
						counter++;
					}
				}
				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {
					byte_read = get_byte();

					switch(byte_nr) {
						case 1: {
							if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
								byte1 = byte_read;
								byte_nr = 2;
							}
							break;
						}
						case 2: {
							byte2 = byte_read;
							byte_nr = 3;
							break;
						}
						case 3: {
							byte3 = byte_read;
							byte_nr = 1;

							data_packet[0] = byte1;
							data_packet[1] = byte2;
							data_packet[2] = byte3;

							print_packet(data_packet);
							break;
						}
					}

					counter = 0;
				}
				break;
			default:
				break;
			}
		} else {
			continue;
		}
	}

	printf("Ended\n");

	timer_unsubscribe_int();
	mouse_unsubscribe_int();
}

int test_config(void) {
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

	mouse_set_stream_mode();
	mouse_status_request();

	while (n_packets < 1) {/*
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {
*/
					byte_read = get_byte();

					switch(byte_nr) {
						case 1: {
							if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
								byte1 = byte_read;
								byte_nr = 2;
							}
							break;
						}
						case 2: {
							byte2 = byte_read;
							byte_nr = 3;
							break;
						}
						case 3: {
							byte3 = byte_read;
							byte_nr = 1;

							data_packet[0] = byte1;
							data_packet[1] = byte2;
							data_packet[2] = byte3;

							n_packets++;
							print_status(data_packet);
							break;
						}
					}
					/*
				}
				break;
			default:
				break;
			}
		} else {
			continue;
		}
	}
*/
	}

	mouse_unsubscribe_int();



}

int test_gesture(short length, unsigned short tolerance) {
	message msg;
	int ipc_status, r;
	long data;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];
	int first_read = 0;

	int line_start = 0;
	short line_length = 0;
	unsigned short deviation = 0;

	if (mouse_subscribe_int() == -1) {
		return 1;
	}

	mouse_set_stream_mode();
	mouse_enable_stream();

	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {
					byte_read = get_byte();

					if (first_read == 0) {
						first_read = 1;
						continue;
					}
					else {
						switch(byte_nr) {
							case 1: {
								if (MOUSE_FIRST_BYTE == (byte_read & MOUSE_FIRST_BYTE)) {
									byte1 = byte_read;
									byte_nr = 2;
								}
								break;
							}
							case 2: {
								byte2 = byte_read;
								byte_nr = 3;
								break;
							}
							case 3: {
								byte3 = byte_read;
								byte_nr = 1;

								data_packet[0] = byte1;
								data_packet[1] = byte2;
								data_packet[2] = byte3;

								print_packet(data_packet);

								// If right button is pressed and wasn't before start a new line
								if ((RB == (data_packet[0] & RB)) && (line_start == 0)) {
									line_start = 1;
								}

								// If a line is being drawn
								if (line_start == 1) {

									// Horizontal deviation
									if (XSIGN == (data_packet[0] & XSIGN)) {
										deviation += (short) (0xff00 |  data_packet[1]);
									} else {
										deviation += data_packet[1];
									}

									// Line length
									if (YSIGN == (data_packet[0] & YSIGN)) {
										line_length += (short) (0xff00 | data_packet[2]);
									} else {
										line_length += data_packet[2];
									}


									// If the line's length is bigger than the condition exit
									if (((length > 0) && (line_length < length)) || ((length < 0) && (line_length > length))) {
										// If the horizontal deviation from vertical is bigger than the tolerance reset
										if (abs(deviation) > tolerance) {
											line_start = 0;
											line_length = 0;
											deviation = 0;
											break;
										}
									} else {
										printf("Terminated\n");
										mouse_unsubscribe_int();
										return 0;
										break;
									}
								}

								break;
							}
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
}
