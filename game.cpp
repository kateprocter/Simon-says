#include "Arduino.h"
#include "game.h"
#include "led.h"
#include "scheduler.h"
#include "botface.h"

#define L_FORWARD       17
#define L_BACKWARD      16
#define R_FORWARD       23
#define R_BACKWARD      22

static ACTION_COMPLETE actionCompleteCallback;

static void MotorsOff(void);
static void SpinL(void);
static void SpinR(void);

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
    SpinL();
    ScheduleTask(EndSpin, 1000, false);
}


void DoForward(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    digitalWrite(L_FORWARD, HIGH);
    digitalWrite(R_FORWARD, HIGH);
    ScheduleTask(EndForward, 500, false);
}
void DoBackward(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    digitalWrite(L_BACKWARD, HIGH);
    digitalWrite(R_BACKWARD, HIGH);
    ScheduleTask(EndBackward, 500, false);
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
    
}

void DoCelebrate(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    SpinL();
    ScheduleTask(SpinR, 250, false);
    ScheduleTask(SpinL, 500, false);
    ScheduleTask(SpinR, 750, false);
    ScheduleTask(SpinL, 1000, false);
    ScheduleTask(SpinR, 1250, false);
    ScheduleTask(NextColour, 100, true);
    ScheduleTask(EndCelebrate, 1500, false);
}

void DoShock(ACTION_COMPLETE callback)
{
    actionCompleteCallback = callback;
    SetMouth(ROBOTMOUTH_SHOCK);
    ScheduleTask(EndShock, 1000, false);
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

void EndCelebrate(void)
{
    MotorsOff();
    CancelTask(NextColour);
    ResetLED();
    (*actionCompleteCallback)();
    
    
}

void EndShock(void)
{
    SetMouth(ROBOTMOUTH_HAPPY);
    (*actionCompleteCallback)();
    
}

void MotorsOff(void)
{
    digitalWrite(L_FORWARD ,LOW);
    digitalWrite(L_BACKWARD ,LOW);
    digitalWrite(R_FORWARD ,LOW);
    digitalWrite(R_BACKWARD,LOW);
}

void SpinL(void)
{
    digitalWrite(L_FORWARD ,HIGH);
    digitalWrite(L_BACKWARD ,LOW);
    digitalWrite(R_FORWARD ,LOW);
    digitalWrite(R_BACKWARD,HIGH);
    
}

void SpinR(void)
{
    digitalWrite(L_FORWARD ,LOW);
    digitalWrite(L_BACKWARD ,HIGH);
    digitalWrite(R_FORWARD ,HIGH);
    digitalWrite(R_BACKWARD,LOW);
}


