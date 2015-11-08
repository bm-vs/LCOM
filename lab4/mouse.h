#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "constants.h"

int mouse_subscribe_int();
int mouse_unsubscribe_int();
int mouse_set_stream_mode();
int mouse_enable_stream();
int print_packet(char data_packet[]);

#endif
