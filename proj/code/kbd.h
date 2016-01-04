#ifndef __KBD_H
#define __KBD_H


/** @defgroup kbd kbd
 * @{
 *
 * Functions related to the keyboard.
 */


/*! \brief Subscribes to the keyboard's interrupts.
*
*	\return the keyboard's hook bit if successful. -1 if not.
*/
int kbd_subscribe_int();

/*! \brief Unsubscribes from the keyboard's interrupts.
*
*	\return 0 if successful. -1 if not.
*/
int timer_unsubscribe_int();


/*! \brief Unsubscribes from the keyboard's interrupts.
*
*	\return 0 if successful. -1 if not.
*/
int kbd_int_handler();



#endif
