#ifndef __KBD_H
#define __KBD_H

int kbd_subscribe_int();

int timer_unsubscribe_int();

int kbd_int_handler();

int kbd_toggle_leds(unsigned short led);


#endif
