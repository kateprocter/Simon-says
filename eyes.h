#ifndef ROBOT_EYES_H
#define ROBOT_EYES_H

#include "Arduino.h"

#define NUM_EYES  40
#define RIGHT_EYE_START_COLUMN     0x15
#define RIGHT_EYE_MID              0x25
#define RIGHT_EYE_END_COLUMN       0x35
#define LEFT_EYE_START_COLUMN    0x4A
#define LEFT_EYE_MID             0x5A
#define LEFT_EYE_END_COLUMN      0x6A
#define EYE_START_PAGE            0x00
#define EYE_END_PAGE              0x04

#define EYE_COLUMNS               ((LEFT_EYE_END_COLUMN - LEFT_EYE_START_COLUMN) + 1)
#define EYE_PAGES                 ((EYE_END_PAGE - EYE_START_PAGE) + 1)
#define EYE_BYTES                 (EYE_COLUMNS * EYE_PAGES)


extern const byte closedeye[EYE_BYTES];
extern const byte eyePics[NUM_EYES][EYE_BYTES];

#endif
