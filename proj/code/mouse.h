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

/** @defgroup mouse mouse
 * @{
 *
 * Functions related to the mouse.
 */

/*! \brief Subscribes to the mouse's interrupts.
*
*	\return the mouse's hook bit if successful. -1 if not.
*/
int mouse_subscribe_int();

/*! \brief Unsubscribes from the mouse's interrupts.
*
*	\return 0 if successful. -1 if not.
*/
int mouse_unsubscribe_int();

/*! \brief Sends the Set Stream Mode command to port 0x64.
*
*	\return 0 if successful. 1 if not.
*/
int mouse_set_stream_mode();

/*! \brief Sends the Enable Sending Data Packets command to port 0x64.
*
*	\return 0 if successful. 1 if not.
*/
int mouse_enable_stream();

/*! \brief Sends the Status Request command to port 0x64.
*
*	\return 0 if successful. 1 if not.
*/
int mouse_status_request();

/*! \brief Receives the data in port 0x60.
*
*	\return the date when successful.
*/
long get_byte();


/*! \brief Receives a mouse data packet and updates the sprite's coordinates accordingly.
*
*	\param data_packet array containing the data from the mouse.
*	\param sprite pointer to a sprite.
*	\param video_mem virtual address VRAM was mapped to.
*
*	\return 1 if the left button is pressed. 0 if not.
*/
int packet_handler(char data_packet[], Sprite *sprite, char *video_mem);

#endif
