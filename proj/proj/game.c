#include "game.h"

// GAME SLIDERS
#define SHOT_SPEED 20
#define TIME_BTW_SHOTS 15
#define PLATFORM1_X 0
#define PLATFORM1_Y 600
#define PLATFORM2_X 723
#define PLATFORM2_Y 600
#define PLATFORM3_X 361
#define PLATFORM3_Y 432
#define PLATFORM4_X 0
#define PLATFORM4_Y 264
#define PLATFORM5_X 723
#define PLATFORM5_Y 264
#define PLAYER1_START_X 300
#define PLAYER1_START_Y 708
#define PLAYER2_START_X 800
#define PLAYER2_START_Y 200
#define CROSSHAIR1_START_X 300
#define CROSSHAIR1_START_Y 700
#define PLAYER_SPEED 7
#define JUMP_SPEED -15
#define PLAYER_HP 5
#define HP_X 900
#define HP_Y 50
#define TIME_X 127
#define TIME_Y 50
#define N_SEC_WIN 3


int game(char *video_mem, int player, int time_other_player) {
	int r, ipc_status, scan_result;
	message msg;
	long data;
	int n_packets = 0;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];
	int counter = 0;
	int last_shot = 0;
	int player_hp = PLAYER_HP;

	kbd_subscribe_int();
	timer_subscribe_int();
	mouse_subscribe_int();

	char* double_buffer;
	char* mouse_buffer;
	double_buffer = malloc(H_RES*V_RES);
	mouse_buffer = malloc(H_RES*V_RES);

	Sprite *platform1 = create_sprite(get_xpm(2), PLATFORM1_X, PLATFORM1_Y);
	draw_sprite(platform1, double_buffer);

	Sprite *platform2 = create_sprite(get_xpm(2), PLATFORM2_X, PLATFORM2_Y);
	draw_sprite(platform2, double_buffer);

	Sprite *platform3 = create_sprite(get_xpm(2), PLATFORM3_X, PLATFORM3_Y);
	draw_sprite(platform3, double_buffer);

	Sprite *platform4 = create_sprite(get_xpm(2), PLATFORM4_X, PLATFORM4_Y);
	draw_sprite(platform4, double_buffer);

	Sprite *platform5 = create_sprite(get_xpm(2), PLATFORM5_X, PLATFORM5_Y);
	draw_sprite(platform5, double_buffer);

	Sprite *player1;
	Sprite *player2;

	if (player == 1) {
		player1 = create_sprite(get_xpm(4), PLAYER1_START_X, PLAYER1_START_Y);
		player2 = create_sprite(get_xpm(5), PLAYER2_START_X, PLAYER2_START_Y);
	}
	else {
		player1 = create_sprite(get_xpm(5), PLAYER1_START_X, PLAYER1_START_Y);
		player2 = create_sprite(get_xpm(4), PLAYER2_START_X, PLAYER2_START_Y);
	}

	draw_sprite(player1, double_buffer);
	memcpy(mouse_buffer, double_buffer, H_RES*V_RES);

	draw_sprite(player2, double_buffer);
	memcpy(mouse_buffer, double_buffer, H_RES*V_RES);

	player2->inplatform = 1;


	Sprite *crosshair = create_sprite(get_xpm(1), CROSSHAIR1_START_X, CROSSHAIR1_START_Y);
	draw_sprite(crosshair, mouse_buffer);
	memcpy(video_mem, mouse_buffer, H_RES*V_RES);

	vector shots1;
	vector_init(&shots1);

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

					// Delete double buffer
					int i;
					for (i = 0; i < H_RES*V_RES; i++) {
						double_buffer[i] = BLACK;
					}

					// Draw platforms
					draw_sprite(platform1, double_buffer);
					draw_sprite(platform2, double_buffer);
					draw_sprite(platform3, double_buffer);
					draw_sprite(platform4, double_buffer);
					draw_sprite(platform5, double_buffer);

					// Move player1
					if (player1->jumping == 1 && (counter % 2 == 0)) {
						set_y_speed_sprite(player1, player1->y_speed + 1);
					}

					move_sprite(player1, H_RES, V_RES, double_buffer);

					// Draw player1
					draw_sprite(player1, double_buffer);


					// Move player2
					if (player2->jumping == 1 && (counter % 2 == 0)) {
						set_y_speed_sprite(player2, player2->y_speed + 1);
					}

					move_sprite(player2, H_RES, V_RES, double_buffer);

					// Draw player2
					draw_sprite(player2, double_buffer);


					// Move shots
					int end = 0;
					for (i = 0; i < shots1.size; i++) {
						// Colision with borders
						int move = move_shot(shots1.items[i], player2, H_RES, V_RES, double_buffer);
						if ((move >= 1) && (move <= 5)) {
							vector_erase(&shots1, i);
						}
						if (move == 6) {
							player_hp--;
							vector_erase(&shots1, i);
							if (player_hp <= 0) {
								end = 1;
							}
						}
					}

					// Draw shots
					for (i = 0; i < shots1.size; i++) {
						draw_sprite(shots1.items[i], double_buffer);
					}

					// Copy double buffer to mouse buffer
					memcpy(mouse_buffer, double_buffer, H_RES*V_RES);

					// Draw crosshair to mouse buffer
					draw_sprite(crosshair, mouse_buffer);

					// Draw player hp
					for (i = 0; i < player_hp; i++) {
						Sprite *hp = create_sprite(get_xpm(6), HP_X - 42*i, HP_Y);
						draw_sprite(hp, mouse_buffer);
						destroy_sprite(hp);
					}

					if (time_other_player/60 < 228 && time_other_player != -1) {
						for (i = 0; i < time_other_player/60; i++) {
							Sprite *time_bar_other = create_sprite(get_xpm(15), TIME_X + 3*i, TIME_Y);
							draw_sprite(time_bar_other, mouse_buffer);
							destroy_sprite(time_bar_other);
						}
					}

					if (counter/60 < 228) {
						for (i = 0; i < counter/60; i++) {
							Sprite *time_bar = create_sprite(get_xpm(14), TIME_X + 3*i, TIME_Y);
							draw_sprite(time_bar, mouse_buffer);
							destroy_sprite(time_bar);
						}
					}

					// Copy mouse buffer to video memory
					memcpy(video_mem, mouse_buffer, H_RES*V_RES);


					if (end == 1) {
						// Unsubscribe
						timer_unsubscribe_int();
						kbd_unsubscribe_int();
						mouse_unsubscribe_int();

						// Free memory
						destroy_sprite(platform1);
						destroy_sprite(platform2);
						destroy_sprite(platform3);
						destroy_sprite(platform4);
						destroy_sprite(platform5);
						destroy_sprite(player1);
						destroy_sprite(player2);
						destroy_sprite(crosshair);
						free(double_buffer);
						free(mouse_buffer);

						return counter;
					}

					counter++;
				}

				//=========================================================================================
				// KEYBOARD

				if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {
						int kbd_receive = kbd_int_handler();


						// Check if its the turn for player 1 to shoot or player 2
						if (player == 1) {


							// MOVE SIDE TO SIDE
							if (kbd_receive == KEY_A) {
								set_x_speed_sprite(player1, -PLAYER_SPEED);
							}
							if (kbd_receive == KEY_D) {
								set_x_speed_sprite(player1, PLAYER_SPEED);
							}


							if (kbd_receive == KEY_J) {
								set_x_speed_sprite(player2, -PLAYER_SPEED);
							}
							if (kbd_receive == KEY_L) {
								set_x_speed_sprite(player2, PLAYER_SPEED);
							}



							// JUMP

							// jump straight
							if ((kbd_receive == KEY_W || kbd_receive == KEY_SPACE) && player1->jumping == 0) {
								set_y_speed_sprite(player1, JUMP_SPEED);
								player1->jumping = 1;
							}


							if ((kbd_receive == KEY_I) && (player2->jumping == 0)) {
								set_y_speed_sprite(player2, JUMP_SPEED);
								player2->jumping = 1;
							}


							// STOP

							if (kbd_receive == BREAK_A || kbd_receive == BREAK_D) {
								set_x_speed_sprite(player1, 0);
							}

							if (kbd_receive == BREAK_J || kbd_receive == BREAK_L) {
								set_x_speed_sprite(player2, 0);
							}
						}

						else {
							// MOVE SIDE TO SIDE
							if (kbd_receive == KEY_A) {
								set_x_speed_sprite(player2, -PLAYER_SPEED);
							}
							if (kbd_receive == KEY_D) {
								set_x_speed_sprite(player2, PLAYER_SPEED);
							}


							if (kbd_receive == KEY_J) {
								set_x_speed_sprite(player1, -PLAYER_SPEED);
							}
							if (kbd_receive == KEY_L) {
								set_x_speed_sprite(player1, PLAYER_SPEED);
							}



							// JUMP

							// jump straight
							if ((kbd_receive == KEY_W || kbd_receive == KEY_SPACE)
									&& player2->jumping == 0) {
								set_y_speed_sprite(player2, JUMP_SPEED);
								player2->jumping = 1;
							}


							if ((kbd_receive == KEY_I) && (player1->jumping == 0)) {
								set_y_speed_sprite(player1, JUMP_SPEED);
								player1->jumping = 1;
							}


							// STOP

							if (kbd_receive == BREAK_A || kbd_receive == BREAK_D) {
								set_x_speed_sprite(player2, 0);
							}

							if (kbd_receive == BREAK_J || kbd_receive == BREAK_L) {
								set_x_speed_sprite(player1, 0);
							}
						}


						if (kbd_receive == ESC) {
							// Unsubscribe
							timer_unsubscribe_int();
							kbd_unsubscribe_int();
							mouse_unsubscribe_int();

							// Free memory
							destroy_sprite(platform1);
							destroy_sprite(platform2);
							destroy_sprite(platform3);
							destroy_sprite(platform4);
							destroy_sprite(platform5);
							destroy_sprite(player1);
							destroy_sprite(player2);
							destroy_sprite(crosshair);
							free(double_buffer);
							free(mouse_buffer);

							return -1;
						}
				}


				//=================================================================================
				// MOUSE

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
							if ((packet_handler(data_packet, crosshair, mouse_buffer) == 1)  &&
									(counter - last_shot >= TIME_BTW_SHOTS)) {
								Sprite *shot = create_sprite(get_xpm(0), player1->x, player1->y);
								int x = player1->x - crosshair->x;
								int y = player1->y - crosshair->y;
								int l = sqrt(x*x + y*y);

								// Avoid crashing by dividing by 0
								if (l >= SHOT_SPEED) {
									int t = l/SHOT_SPEED;

									shot->x_speed = -x/t;
									shot->y_speed = -y/t;

									vector_push_back(&shots1, shot);
									last_shot = counter;
								}
							}
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
}


int menu(char *video_mem){
	int r, ipc_status, scan_result;
	message msg;
	long data;
	int n_packets = 0;
	int byte_nr = 1;
	char byte_read, byte1, byte2, byte3;
	char data_packet[3];

	char* mouse_buffer;
	char* double_buffer;

	//Subscribes
	timer_subscribe_int();
	mouse_subscribe_int();
	kbd_subscribe_int();

	//Initialization
	double_buffer = malloc(H_RES*V_RES);
	mouse_buffer = malloc(H_RES*V_RES);

	// Background
	int i;
	for(i = 0;i <H_RES*V_RES;i++){
		double_buffer[i]=0;
	}

	// Number and letters
	Sprite *pewpew1 = create_sprite(get_xpm(7), 264, 100);
	draw_sprite(pewpew1, double_buffer);
	destroy_sprite(pewpew1);

	Sprite *pewpew2 = create_sprite(get_xpm(7), 370, 206);
	draw_sprite(pewpew2, double_buffer);
	destroy_sprite(pewpew2);

	Sprite *ex = create_sprite(get_xpm(13), 736, 206);
	draw_sprite(ex, double_buffer);
	destroy_sprite(ex);


	Sprite *start = create_sprite(get_xpm(8), 354, 500);
	draw_sprite(start, double_buffer);


	memcpy(mouse_buffer, double_buffer, H_RES * V_RES);

	//Mouse cursor
	Sprite *arrow = create_sprite(get_xpm(3), 50,50);
	draw_sprite(arrow, mouse_buffer);

	//Mouse stream and enable
	mouse_set_stream_mode();
	mouse_enable_stream();

	memcpy(video_mem,mouse_buffer, H_RES * V_RES);

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

					// Copy double buffer to mouse buffer
					memcpy(mouse_buffer, double_buffer, H_RES * V_RES);

					// Draw crosshair to mouse buffer
					draw_sprite(arrow, mouse_buffer);

					// Copy mouse buffer to video memory
					memcpy(video_mem, mouse_buffer, H_RES * V_RES);
				}


				//=================================================================================
				// KEYBOARD

				if (msg.NOTIFY_ARG & BIT(KBD_HOOK_BIT)) {
					if (kbd_int_handler() == ESC) {
						timer_unsubscribe_int();
						mouse_unsubscribe_int();
						kbd_unsubscribe_int();

						// Free memory
						destroy_sprite(arrow);
						destroy_sprite(start);
						free(double_buffer);
						free(mouse_buffer);

						return -1;
					}
				}



				//=================================================================================
				// MOUSE

				if (msg.NOTIFY_ARG & BIT(MOUSE_HOOK_BIT)) {

					byte_read = get_byte();

					switch (byte_nr) {
					case 1: {
						if (MOUSE_FIRST_BYTE
								== (byte_read & MOUSE_FIRST_BYTE)) {
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
						if (packet_handler(data_packet, arrow, mouse_buffer)== 1) {
							int x = arrow->x;
							int y = arrow->y;
							if(x >= 354 && x<=673 && y >=500 && y<=560){
								timer_unsubscribe_int();
								mouse_unsubscribe_int();
								kbd_unsubscribe_int();

								// Free memory
								destroy_sprite(arrow);
								destroy_sprite(start);
								free(double_buffer);
								free(mouse_buffer);

								return 0;
							}
						}
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
}

void player_wins(char *video_mem, int player) {
	int r, ipc_status, scan_result;
	message msg;
	long data;
	int counter = 0;

	char* double_buffer;

	timer_subscribe_int();

	double_buffer = malloc(H_RES*V_RES);

	Sprite *pl;
	pl = create_sprite(get_xpm(9), 62, 320);
	draw_sprite(pl, double_buffer);
	destroy_sprite(pl);

	Sprite *n;
	Sprite *w;
	if (player == 1) {
		n = create_sprite(get_xpm(10), 562, 320);
		draw_sprite(n, double_buffer);

		w = create_sprite(get_xpm(12), 641, 320);
		draw_sprite(w, double_buffer);

		destroy_sprite(n);
		destroy_sprite(w);
	}
	else {
		n = create_sprite(get_xpm(11), 562, 320);
		draw_sprite(n, double_buffer);

		w = create_sprite(get_xpm(12), 686, 320);
		draw_sprite(w, double_buffer);

		destroy_sprite(n);
		destroy_sprite(w);
	}




	memcpy(video_mem, double_buffer, H_RES * V_RES);

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
						if (counter >= 60*N_SEC_WIN) {
							free(double_buffer);
							return;
						}

						counter++;
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
