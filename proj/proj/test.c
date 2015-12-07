#include "test.h"

void move_character() {
	if (kbd_subscribe_int() != 1) {
		return 1;
	}
	if (timer_subscribe_int() != 0) {
		return 1;
	}

	char* video_mem;
	video_mem = vg_init(VBE_105_MODE);
	Sprite *sprite = create_sprite(get_pixmap(4), 300, 700);
	draw_sprite(sprite, video_mem);


	int counter = 0;

	int r, ipc_status, scan_result;
	message msg;

	while (1) {
		counter++;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & BIT(0)) {
						erase_sprite(sprite, video_mem);

						//=================================================================================
						// MOVE SIDE TO SIDE
						if (kbd_int_handler() == KEY_A) {
							set_x_speed_sprite(sprite, -7);
							move_sprite(sprite, H_RES, V_RES, video_mem);
							set_x_speed_sprite(sprite, 0);
						}
						if (kbd_int_handler() == KEY_D) {
							set_x_speed_sprite(sprite, 7);
							move_sprite(sprite, H_RES, V_RES, video_mem);
							set_x_speed_sprite(sprite, 0);
						}


						//=================================================================================
						// JUMP

						// jump left
						if (kbd_int_handler() == KEY_Q && sprite->jumping == 0) {
							set_y_speed_sprite(sprite, -20);
							set_x_speed_sprite(sprite, -7);
							sprite->jumping = 1;
						}

						// jump straight
						if (kbd_int_handler() == KEY_W && sprite->jumping == 0) {
							set_y_speed_sprite(sprite, -20);
							sprite->jumping = 1;
						}

						// jump right
						if (kbd_int_handler() == KEY_E && sprite->jumping == 0) {
							set_y_speed_sprite(sprite, -20);
							set_x_speed_sprite(sprite, 7);
							sprite->jumping = 1;
						}


						if (check_collision_sprite(sprite, H_RES, V_RES, video_mem) == 1) {
							sprite->jumping = 0;
							set_y_speed_sprite(sprite, 0);
							set_x_speed_sprite(sprite, 0);
						}
						if (check_collision_sprite(sprite, H_RES, V_RES, video_mem) == 2) {
							set_x_speed_sprite(sprite, 0);
						}

						if (sprite->jumping == 1) {
							move_sprite(sprite, H_RES, V_RES, video_mem);
							set_y_speed_sprite(sprite, sprite->y_speed + 1);
						}

						//=================================================================================
						// SHOOT





						draw_sprite(sprite, video_mem);
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

	return 1;
}

void shoot() {

}
