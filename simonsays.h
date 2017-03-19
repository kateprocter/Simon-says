#ifndef SIMONSAYS_H
#define SIMONSAYS_H

#define ROBOT_NOP       0x00

#define SIMON_SAYS      0x10
#define UNSIMON_SAYS    0x20
#define MUST            0x40

#define COMMAND_MASK    0x0F

#define SIMON_SPIN      0x00
#define SIMON_FORWARD   0x01
#define SIMON_BACKWARD  0x02
#define SIMON_FLASH     0x03
#define SIMON_RAINBOW   0x04
#define SIMON_SLEEP     0x05
#define SIMON_BLINK     0x06
#define SIMON_CROSS_EYE 0x07
#define SIMON_MAX       0x08

#define GO_IDLE         0x09
#define THIS_BOT_OUT    0x0A
#define OTHER_BOT_OUT   0x0B
#define BOT_WINS        0x0C

#define I_AM_BUSY       0x01
#define I_AM_IDLE       0x02
#define I_AM_WRONG      0x03

typedef enum
{
    ROBOT_BUSY,
    ROBOT_IDLE,
    ROBOT_WRONG,
}ROBOT_STATE;

#endif





