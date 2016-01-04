#ifndef __GAME_H
#define __GAME_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/sysutil.h>
#include <minix/com.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "timer.h"
#include "kbd.h"
#include "video_gr.h"
#include "sprite.h"
#include "vector.h"
#include "vbe.h"


int game(char *video_mem, int player, int time_other_player);

int menu(char *video_mem);

void player_wins(char *video_mem, int player);

#endif
