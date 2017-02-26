#include "Arduino.h"
#include "led.h"
#include "scheduler.h"

#define R_RED   0
#define R_GREEN 1
#define R_BLUE  2

#define L_RED   3
#define L_GREEN 4
#define L_BLUE  5


static LED_COLOUR myColour;
static LED_COLOUR currentColour;
static bool flash;

void InitLED(LED_COLOUR colour)
{
    pinMode(L_RED, OUTPUT);
    pinMode(L_BLUE, OUTPUT);
    pinMode(L_GREEN, OUTPUT);
    pinMode(R_RED, OUTPUT);
    pinMode(R_BLUE, OUTPUT);
    pinMode(R_GREEN, OUTPUT);

    SetLED(true, true, colour);
    myColour = colour;
    currentColour = colour;

}

void ResetLED(void)
{
    SetLED(true, true, myColour);
    CancelTask(NextColour);
    CancelTask(FlashLED);
}

void FlashLED(void)
{
    if(flash)
    {
        SetLED(true, false, currentColour);
        SetLED(false, true, LED_OFF);
    }
    else
    {
        SetLED(false, true, currentColour);
        SetLED(true, false, LED_OFF);
    }
   
    flash = !flash;
}

void NextColour(void)
{
    currentColour = LED_COLOUR((currentColour + 1) % (LED_MAX_COLOUR - 1));
    SetLED(true, true, currentColour);
}

void SetLED(bool left, bool right, LED_COLOUR ledColour)
{
    bool r = false;
    bool g = false;
    bool b = false;

    if(ledColour != LED_OFF)
    {
        currentColour = ledColour;
    }

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
    default:
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
