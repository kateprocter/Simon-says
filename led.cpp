#include "Arduino.h"
#include "led.h"

#define L_RED   0
#define L_GREEN 1
#define L_BLUE  2

#define R_RED   3
#define R_GREEN 4
#define R_BLUE  5


void InitLED(void)
{
    pinMode(L_RED, OUTPUT);
    pinMode(L_BLUE, OUTPUT);
    pinMode(L_GREEN, OUTPUT);
    pinMode(R_RED, OUTPUT);
    pinMode(R_BLUE, OUTPUT);
    pinMode(R_GREEN, OUTPUT);

}


void SetLED(bool left, bool right, LED_COLOUR ledColour)
{
    bool r = false;
    bool g = false;
    bool b = false;

    switch(ledColour)
    {
    case LED_RED:
        r=true;
        g=false;
        b=false;
        break;
    case LED_GREEN:
        r=false;
        g=true;
        b=false;
        break;
    case LED_BLUE:
        r=false;
        g=false;
        b=true;
        break;
    case LED_CYAN:
        r=false;
        g=true;
        b=true;
        break;
    case LED_YELLOW:
        r=true;
        g=true;
        b=false;
        break;
    case LED_MAGENTA:
        r=true;
        g=false;
        b=true;
        break;
    case LED_WHITE:
        r=true;
        g=true;
        b=true;
        break;
    case LED_OFF:
        r=false;
        g=false;
        b=false;
        break;
    }
    if(left)
    {
        digitalWrite(L_RED, r);
        digitalWrite(L_BLUE, b);
        digitalWrite(L_GREEN,g);
    }

    if(right)
    {
        digitalWrite(R_RED,r);
        digitalWrite(R_GREEN, g);
        digitalWrite(R_BLUE, b);
    }
}
