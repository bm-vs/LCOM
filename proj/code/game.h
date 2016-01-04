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

/** @defgroup game game
 * @{
 *
 * Functions related to game and user interface.
 */


/*! \brief Primary game function.
*	
*	Responsible for a player's turn.
*	After a turn starts it lasts until who player who is running away loses all its health.
*
*	\param video_mem virtual address VRAM was mapped to.
*	\param player number of the player that will shoot this turn.
*	\param time_other_player time the other player took completing his turn (in ms). If no other player as completed a turn this parameter should be -1.
*	\return number of seconds the turn took. -1 if the the ESC key was pressed.
*/
int game(char *video_mem, int player, int time_other_player);

/*! \brief Function for displaying the menu.
*	
*	Responsible for displaying the menu screen and either indicate the beginning of the game or exit of the program.
*
*	\param video_mem virtual address VRAM was mapped to.
*	\return 0 if the user chose to begin the game. -1 if the ESC key was pressed.
*/
int menu(char *video_mem);

/*! \brief Function for displaying the winner of the game.
*	
*	Responsible for displaying the player that took the less amount of time to complete its turn.
*
*	\param video_mem virtual address VRAM was mapped to.
*	\param player number of the player that won the game.
*/
void player_wins(char *video_mem, int player);

#endif
