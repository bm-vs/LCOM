#ifndef __TIMER_H
#define __TIMER_H


int timer_set_square(unsigned long timer, unsigned long freq);

int timer_subscribe_int(void );

int timer_unsubscribe_int();


#endif /* __TIMER_H */
