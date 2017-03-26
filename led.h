#ifndef LED_H
#define LED_H

typedef enum
{
    LED_RED = 0,
    LED_GREEN,
    LED_BLUE,
    LED_MAGENTA,
    LED_YELLOW,
    LED_CYAN,
    LED_WHITE,
    LED_OFF,
    LED_MAX_COLOUR
}LED_COLOUR;

extern void InitLED(LED_COLOUR colour);
extern void ResetLED(void);
extern void NextColour(void);
extern void FlashLED(void);
extern void SetLED(bool left, bool right, LED_COLOUR colour); 

#endif
