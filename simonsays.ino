#include "botface.h"
#include "led.h"

#define UPDATE_PERIOD_MS   10

void setup()
{
    InitBotFace();
    InitLED();

    SetLED(true, true, LED_MAGENTA);
}


void loop()
{
    unsigned long startTime = millis();
    
    UpdateEyes();

    while(millis() - startTime < UPDATE_PERIOD_MS);
}
