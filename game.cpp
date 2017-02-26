#include "Arduino.h"
#include "game.h"
#include "led.h"
#include "scheduler.h"
#include "botface.h"

#define L_FORWARD       16 
#define L_BACKWARD      17
#define R_FORWARD       22
#define R_BACKWARD      23

static ACTION_COMPLETE actionCompleteCallback;

static void MotorsOff(void);

void InitGame(void)
{
    pinMode(L_FORWARD, OUTPUT);
    pinMode(L_BACKWARD, OUTPUT);
    pinMode(R_FORWARD, OUTPUT);
    pinMode(R_BACKWARD, OUTPUT);

    
}

void DoSpin(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    digitalWrite(L_FORWARD, HIGH);
    ScheduleTask(EndSpin, 2000, false);
}


void DoForward(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    digitalWrite(L_FORWARD, HIGH);
    digitalWrite(R_FORWARD, HIGH);
    ScheduleTask(EndForward, 1000, false);
}
void DoBackward(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    digitalWrite(L_BACKWARD, HIGH);
    digitalWrite(R_BACKWARD, HIGH);
    ScheduleTask(EndBackward, 1000, false);
}

void DoFlash(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    ScheduleTask(FlashLED, 250, true);
    ScheduleTask(EndFlash, 3000, false);
}

void DoRainbow(ACTION_COMPLETE callback)
{
   actionCompleteCallback = callback;
   ScheduleTask(NextColour, 250, true);
   ScheduleTask(EndRainbow, 3000, false);
}

void DoSleep(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    QueueEyeAction(EYE_COMMAND_SLEEP, 0, 0, 4000, NULL);
    QueueEyeAction(EYE_COMMAND_WAKE, 0, 0, 0, EndSleep);

}

void DoBlink(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    QueueEyeAction(EYE_COMMAND_BLINK, 0, 0, 0, NULL);
    QueueEyeAction(EYE_COMMAND_BLINK, 0, 0, 0, NULL);
    QueueEyeAction(EYE_COMMAND_BLINK, 0, 0, 0, EndBlink);
    
}

void DoCrossEye(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    QueueEyeAction(EYE_COMMAND_LOOK, 0,0,0, NULL);
    QueueEyeAction(EYE_COMMAND_LOOK, 32, 8, 100, NULL);
    QueueEyeAction(EYE_COMMAND_LOOK, 0, 0, 0, EndCrossEye);
    actionCompleteCallback = callback;
    
}

void EndSpin(void)
{
   MotorsOff();
   (*actionCompleteCallback)();
}


void EndForward(void)
{
    MotorsOff();
   (*actionCompleteCallback)();
}
void EndBackward(void)
{
    MotorsOff();
   (*actionCompleteCallback)();
}

void EndFlash(void)
{
   CancelTask(FlashLED);
   ResetLED(); (*actionCompleteCallback)();

}

void EndRainbow(void)
{
    CancelTask(NextColour);
    ResetLED();
    (*actionCompleteCallback)();
}

void EndSleep(void)
{
    (*actionCompleteCallback)();
}

void EndBlink(void)
{
    (*actionCompleteCallback)();
}

void EndCrossEye(void)
{
    (*actionCompleteCallback)();
}

void MotorsOff(void)
{
    digitalWrite(L_FORWARD ,LOW);
    digitalWrite(L_BACKWARD ,LOW);
    digitalWrite(R_FORWARD ,LOW);
    digitalWrite(R_BACKWARD,LOW);
}

