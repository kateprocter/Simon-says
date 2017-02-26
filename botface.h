#ifndef BOTFACE_H
#define BOTFACE_H

#include "mouth.h"

#define BOTFACE_ADDRESS 0x3C

typedef enum
{
    EYE_COMMAND_LOOK,
    EYE_COMMAND_BLINK,
    EYE_COMMAND_SLEEP,
    EYE_COMMAND_WAKE,
    EYE_COMMAND_PAUSE,
    
}EYE_COMMAND;

//Control
#define BOTFACE_CONTROL_CMD_SINGLE          0x80
#define BOTFACE_CONTROL_CMD_STREAM          0x00
#define BOTFACE_CONTROL_DATA_SINGLE         0xC0
#define BOTFACE_CONTROL_DATA_STREAM         0x40

//Fundamental commands
#define BOTFACE_CMD_SET_CONTRAST_CTRL       0x81
#define BOTFACE_CMD_DISPLAY_RAM             0xA4
#define BOTFACE_CMD_DISPLAY_ALLON           0xA5
#define BOTFACE_CMD_DISPLAY_NORMAL          0xA6
#define BOTFACE_CMD_DISPLAY_INVERSE         0xA7
#define BOTFACE_CMD_DISPLAY_ON              0xAF
#define BOTFACE_CMD_DISPLAY_OFF             0xAE

//Addressing
#define BOTFACE_CMD_SET_MEMORY_ADDR_MODE    0x20
#define BOTFACE_CMD_HORIZONTAL_MODE         0x00
#define BOTFACE_CMD_VERTICAL_MODE           0x01
#define BOTFACE_CMD_PAGE_MODE               0x02

#define BOTFACE_CMD_SET_COLUMN_RANGE        0x21
#define BOTFACE_CMD_SET_PAGE_RANGE          0x22
#define BOTFACE_CMD_SET_PAGE_START_MASK     0xB0          


//Hardware configuration

#define BOTFACE_CMD_DISPLAY_START_MASK      0x40
#define BOTFACE_CMD_SET_SEGMENT_REMAP       0xA1
#define BOTFACE_CMD_CLEAR_SEGMENT_REMAP     0xA0
#define BOTFACE_CMD_SET_MUX_RATIO           0xA8
#define BOTFACE_CMD_SET_COM_SCAN_MODE       0xC8
#define BOTFACE_CMD_SET_DISPLAY_OFFSET      0xD3
#define BOTFACE_CMD_SET_COM_PIN_MAP         0xDA

#define BOTFACE_CMD_NOP                     0xE3

//Timing and driving
#define BOTFACE_CMD_SET_CLOCKDIV            0xD5
#define BOTFACE_CMD_SET_PRECHARGE           0xD9
#define BOTFACE_CMD_SET_VCOMH_DESELECT           0xDB
#define BOTFACE_CMD_SET_CHARGEPUMP          0x8D


#define BOTFACE_LAST_COL                   0x7F
#define BOTFACE_LAST_PAGE                   0x07

#define BOTFACE_TOTAL_BYTES                ((BOTFACE_LAST_COL + 1) * (BOTFACE_LAST_PAGE + 1))

#define BOTFACE_NUM_ROWS    64

extern void InitBotFace(void);
extern void SetMouth(ROBOT_MOUTH mouth);
extern void UpdateEyes(void);
extern bool QueueEyeAction(EYE_COMMAND action, int leftTarget, int rightTarget, int pause, void (*callback)(void));


#endif
