#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "timer.h"
#include "sprite.h"


int mouse_subscribe_int();

int mouse_unsubscribe_int();

int mouse_set_stream_mode();

int mouse_enable_stream();

int mouse_status_request();

long get_byte();

int packet_handler(char data_packet[], Sprite *sprite, char *video_mem);

#endif
