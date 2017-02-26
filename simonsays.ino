#include <SPI.h>
#include <EEPROM.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "botface.h"
#include "led.h"
#include "scheduler.h"
#include "simonsays.h"
#include "game.h"

#define EEPROM_SEED 0
#define EEPROM_COL 1

static bool robotBusy = false;
static bool robotOut = false;




void setup()
{
    int randSeed;

    randSeed = EEPROM.read(0);
    randomSeed(randSeed);
    EEPROM.write(EEPROM_SEED, random(0xFF));
    
    InitLED(LED_COLOUR(EEPROM.read(EEPROM_COL)));
    InitBotFace();
    InitScheduler();

    ScheduleTask(UpdateEyes, 10, true);
    RunScheduler();

    Serial.begin(9600);
    
}



void loop()
{

    int b;
    
    if(!robotBusy)
    {
        if(Serial.available())
        {
            b=Serial.read();
            Serial.println(b);
            parseGameMove(b);
        }
    }

}

bool parseGameMove(byte message)
{
 
    switch(message)
    {
    case 'A':
        SimonSays(0);
        break;

    case 'B':
        SimonSays(1);
        break;

    case 'C':
        SimonSays(2);
        break;

    case 'D':
        SimonSays(3);
        break;

    case 'E':

        SimonSays(4);
        break;
        
    case 'F':
        SimonSays(5);
        break;

    case 'G':
        SimonSays(6);
        break;

    case 'H':
        SimonSays(7);
        break;

    case 'I':
        NoSimon(0);
        break;

    case 'J':
        NoSimon(1);
        break;

    case 'K':

        NoSimon(2);
        break;
        
    case 'L':
        NoSimon(3);
        break;
        
     case 'M':
        NoSimon(4);
        break;

    case 'N':
        NoSimon(5);
        break;

    case 'O':

        NoSimon(6);
        break;
        
    case 'P':
        NoSimon(7);
        break;

    case 'X':
        RobotOut();
        break;
    default:
        break;
    
    }

 /*
    if(message & SIMON_SAYS_INSTRUCTION)
    {
        SetMouth(ROBOTMOUTH_HAPPY);
        SimonSays(message & 0x0F);
        robotBusy = true;
    }
    else if(message & NO_SIMON_INSTRUCTION)
    {
        SetMouth(ROBOTMOUTH_HAPPY);
        NoSimon(message & 0x0F);
        robotBusy = true;
    }
    else if(message  == (ROBOT_COMMAND | ROBOT_OUT))
    {
        RobotOut();
    }
    */
}

bool SimonSays(int instruction)
{
    int obey = random(0,5);
    SetMouth(ROBOTMOUTH_HAPPY);

    if(obey == 0)
    {
        instruction  = (instruction + random(1,SIMON_MAX)) % SIMON_MAX;
        robotOut = true;
    }
    else
    {
        robotOut = false;
    }

   GameMove(instruction);

    return ((obey == 0) ? false : true);
    
}

bool NoSimon(int instruction)
{
    int obey = random(0,5);

    SetMouth(ROBOTMOUTH_HAPPY);

    if(obey == 0)
    {
       GameMove(instruction);
       robotOut = true;
    }
    else
    {
        robotOut = false;
    }

    return ((obey == 0) ? false : true);
    
}

void GameMove(int instruction)
{
    switch(instruction)
    {
    case SIMON_SPIN:
        DoSpin(GameMoveComplete);
        break;
          
    case SIMON_FORWARD:
        DoForward(GameMoveComplete);
        break;
        
    case SIMON_BACKWARD:
        DoBackward(GameMoveComplete);
        break;
        
    case SIMON_FLASH:
        DoFlash(GameMoveComplete);
        break;
        
    case SIMON_RAINBOW:
        DoRainbow(GameMoveComplete);
        break;
        
    case SIMON_SLEEP:
        DoSleep(GameMoveComplete);
        break;
        
    case SIMON_BLINK:
        DoBlink(GameMoveComplete);
        break;
        
    case SIMON_CROSS_EYE:
        DoCrossEye(GameMoveComplete);
        break;

    default:
        break;

    }
    
}

void GameMoveComplete(void)
{
    Serial.println("Game move complete");
    
    if(robotOut)
    {
        Serial.println("error!");
        SetMouth(ROBOTMOUTH_CONFUSE);
        robotBusy = false;

    }
    else
    {
        Serial.println("OK");
        robotBusy = false;
    }
}

void RobotOut(void)
{
    SetMouth(ROBOTMOUTH_SAD);
}
  

