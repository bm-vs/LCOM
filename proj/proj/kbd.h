#ifndef __KBD_H
#define __KBD_H

int kbd_subscribe_int();

int timer_unsubscribe_int();

int kbd_int_handler();

// returns 0 if the esc breakcode is read
int kbd_scan_esc();


#endif
