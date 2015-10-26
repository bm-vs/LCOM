#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define BIT(n) (0x01<<(n))

#define KBD_IRQ			1
#define KBD_HOOK_BIT	1

#define STAT_REG		0x64
#define OUT_BUFF		0x60

#define TOGGLE_LEDS		0xED

#define BREAKCODE		0x80

#define ESC				0x81

#define SCROLL_LOCK		BIT(0)
#define NUM_LOCK		BIT(1)
#define CAPS_LOCK		BIT(2)


#endif
