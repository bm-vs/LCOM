#include "test.h"

void move_and_shoot() {
	int r, ipc_status, scan_result;
	message msg;
	long data;
	int n_packets = 0;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];


	if (kbd_subscribe_int() != 1) {
		return 1;
	}
	if (timer_subscribe_int() != 0) {
		return 1;
	}
	if (mouse_subscribe_int() == -1) {
		return 1;
	}

	char* video_mem;
	char* double_buffer;
	char* mouse_buffer;
	video_mem = vg_init(VBE_105_MODE);
	double_buffer = malloc(H_RES*V_RES);
	mouse_buffer = malloc(H_RES*V_RES);

	Sprite *character = create_sprite(get_pixmap(4), 300, 700);
	draw_sprite(character, double_buffer);
	memcpy(mouse_buffer, double_buffer, H_RES*V_RES);

	Sprite *crosshair = create_sprite(get_pixmap(2), 300, 700);
	draw_sprite(crosshair, mouse_buffer);
	memcpy(video_mem, mouse_buffer, H_RES*V_RES);

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

				//=========================================================================================
				// TIMER

				if (msg.NOTIFY_ARG & BIT(TIMER_HOOK_BIT)) {
					erase_sprite(character, double_buffer);
					move_sprite(character, H_RES, V_RES, double_buffer);

					if (check_collision_sprite(character, H_RES, V_RES, double_buffer) == 1) {
						character->jumping = 0;
						set_y_speed_sprite(character, 0);
						set_x_speed_sprite(character, 0);
					}
					if (check_collision_sprite(character, H_RES, V_RES, double_buffer) == 2) {
						set_x_speed_sprite(character, 0);
					}

					if (character->jumping == 1) {
						move_sprite(character, H_RES, V_RES, double_buffer);
						set_y_speed_sprite(character, character->y_speed + 1);
					}


					draw_sprite(character, double_buffer);
					memcpy(mouse_buffer, double_buffer, H_RES*V_RES);

					draw_sprite(crosshair, mouse_buffer);
					memcpy(video_mem, mouse_buffer, H_RES*V_RES);
				}

				//=========================================================================================
				// KEYBOARD

				if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {

						// MOVE SIDE TO SIDE
						if (kbd_int_handler() == KEY_A) {
							set_x_speed_sprite(character, -7);
						}
						if (kbd_int_handler() == KEY_D) {
							set_x_speed_sprite(character, 7);
						}



						// JUMP

						// jump straight
						if ((kbd_int_handler() == KEY_W || kbd_int_handler() == KEY_SPACE) && character->jumping == 0) {
							set_y_speed_sprite(character, -15);
							character->jumping = 1;
						}


						// STOP

						if (kbd_int_handler() == BREAK_A || kbd_int_handler() == BREAK_D) {
							set_x_speed_sprite(character, 0);
						}
				}


				//=================================================================================
				// MOUSE

				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {

					erase_sprite(crosshair, mouse_buffer);
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
							packet_handler(data_packet, crosshair, mouse_buffer);
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


	if (timer_unsubscribe_int() != 0) {
		return 1;
	}


	kbd_unsubscribe_int();

	mouse_unsubscribe_int();

	return 1;
}

void move () {
	int r, ipc_status, scan_result;
	message msg;

	if (kbd_subscribe_int() != 1) {
		return 1;
	}
	if (timer_subscribe_int() != 0) {
		return 1;
	}

	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);
	Sprite *character = create_sprite(get_pixmap(4), 300, 700);
	draw_sprite(character, video_mem);

	while (1) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & BIT(TIMER_HOOK_BIT)) {
						erase_sprite(character, video_mem);

						//=================================================================================
						// MOVE SIDE TO SIDE
						if (kbd_int_handler() == KEY_A) {
							set_x_speed_sprite(character, -7);
							move_sprite(character, H_RES, V_RES, video_mem);
							set_x_speed_sprite(character, 0);
						}
						if (kbd_int_handler() == KEY_D) {
							set_x_speed_sprite(character, 7);
							move_sprite(character, H_RES, V_RES, video_mem);
							set_x_speed_sprite(character, 0);
						}


						//=================================================================================
						// JUMP

						// jump left
						if (kbd_int_handler() == KEY_Q && character->jumping == 0) {
							set_y_speed_sprite(character, -20);
							set_x_speed_sprite(character, -7);
							character->jumping = 1;
						}

						// jump straight
						if (kbd_int_handler() == KEY_W && character->jumping == 0) {
							set_y_speed_sprite(character, -20);
							character->jumping = 1;
						}

						// jump right
						if (kbd_int_handler() == KEY_E && character->jumping == 0) {
							set_y_speed_sprite(character, -20);
							set_x_speed_sprite(character, 7);
							character->jumping = 1;
						}


						if (check_collision_sprite(character, H_RES, V_RES, video_mem) == 1) {
							character->jumping = 0;
							set_y_speed_sprite(character, 0);
							set_x_speed_sprite(character, 0);
						}
						if (check_collision_sprite(character, H_RES, V_RES, video_mem) == 2) {
							set_x_speed_sprite(character, 0);
						}

						if (character->jumping == 1) {
							move_sprite(character, H_RES, V_RES, video_mem);
							set_y_speed_sprite(character, character->y_speed + 1);
						}

						//=================================================================================
						// SHOOT





						draw_sprite(character, video_mem);
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


	kbd_unsubscribe_int();
}

void shoot() {
	int r, ipc_status, scan_result;
	message msg;
	long data;
	int counter = 0;
	int n_packets = 0;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];

	if (mouse_subscribe_int() == -1) {
		return 1;
	}

	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);
	Sprite *crosshair = create_sprite(get_pixmap(2), 300, 300);
	draw_sprite(crosshair, video_mem);

	mouse_set_stream_mode();
	mouse_enable_stream();

	while (1) {
		counter++;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {
					erase_sprite(crosshair, video_mem);
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
							packet_handler(data_packet, crosshair, video_mem);
							/*
							if (packet_handler(data_packet, sprite, video_mem) == 1) {
								// Projectile




							}*/
							draw_sprite(crosshair, video_mem);
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
