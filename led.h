typedef enum
{
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_CYAN,
    LED_YELLOW,
    LED_MAGENTA,
    LED_WHITE,
    LED_OFF
}LED_COLOUR;

extern void InitLED(void);
extern void SetLED(bool left, bool right, LED_COLOUR colour); 
