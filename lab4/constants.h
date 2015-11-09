#ifndef __CONSTANTS_H
#define __CONSTANTS_H

#define BIT(n) (0x01<<(n))

#define TIMER_HOOK_BIT	0
#define MOUSE_HOOK_BIT	1
#define MOUSE_IRQ	12

#define MOUSE_FIRST_BYTE	BIT(3)

#define WRITE_TO_MOUSE	0xD4
#define SET_STREAM_MODE	0xEA
#define DATA_STREAM_ON	0xF4
#define STATUS_REQUEST 0xE9

#define ACK  0xFA

#define OUT_BUFF	0x60
#define STAT_REG	0x64

#define IBF	BIT(1)
#define OBF BIT(0)

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define XSIGN BIT(4)
#define YSIGN BIT(5)
#define XOV BIT(6)
#define YOV BIT(7)

#define REMOTE BIT(6)
#define ENABLE BIT(5)
#define SCALING BIT(4)
#define LEFT BIT(2)
#define MIDDLE BIT(1)
#define RIGHT BIT(0)



#define WAIT_KBC 2000

#endif
