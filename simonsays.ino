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

RF24 radio(14,10);

byte address[] = {0xCA, 0xCA, 0xCA, 0xCA, 0x01};

ROBOT_STATE robotState;
bool        robotObey;


void setup()
{
    int randSeed;

    randSeed = EEPROM.read(0);
    randomSeed(randSeed);
    EEPROM.write(EEPROM_SEED, random(0xFF));

    Serial.begin(9600);
    delay(2000);
    
    radio.begin();
    radio.setAutoAck(1);                    // Ensure autoACK is enabled
    radio.enableAckPayload();               // Allow optional ack payloads
    radio.enableDynamicPayloads();
    radio.setRetries(5,15);                 // Smallest time between retries, max no. of retries
    radio.openReadingPipe(1,address);
    radio.printDetails();
    radio.startListening();                 // Start listening
    
    InitLED(LED_COLOUR(EEPROM.read(EEPROM_COL)));
    InitBotFace();
    InitScheduler();

    ScheduleTask(UpdateEyes, 10, true);
    RunScheduler();

    robotState = ROBOT_IDLE;
  
}



void loop()
{
    byte packet;
    byte rxpipe;
    byte response;

    if(radio.available(&rxpipe))
    {
        while(radio.available(&rxpipe))
        {
            radio.read(&packet,1);
        }

        Serial.println("Got packet");

        Serial.println(packet);
        doCommand(packet);
    
        switch(robotState)
        {
        case ROBOT_IDLE:
            response = I_AM_IDLE;
            break;
        case ROBOT_BUSY:
            response = I_AM_BUSY;
            break;
        case ROBOT_WRONG:
            response = I_AM_WRONG;
            break;
        }
    
        radio.writeAckPayload(1, &response, 1);
    }

}

void doCommand(byte packet)
{
    if((packet & SIMON_SAYS) && (robotState == ROBOT_IDLE))
    {
        SimonSays(packet & COMMAND_MASK);
    }
    else if ((packet & UNSIMON_SAYS) && (robotState == ROBOT_IDLE))
    {
        SimonSays(packet & COMMAND_MASK);
    }
    else if(packet & MUST)
    {
        SimonSays(packet & COMMAND_MASK);
    }
}

void mustDo(int instruction)
{
    switch (instruction)
    {
    case GO_IDLE:
        SetMouth(ROBOTMOUTH_HAPPY);
        robotState = ROBOT_IDLE;
        break;
    case THIS_BOT_OUT:
        SetMouth(ROBOTMOUTH_SAD);
        robotState = ROBOT_BUSY;
        break;
    case OTHER_BOT_OUT:
        robotObey = true;
        DoShock(GameMoveComplete);
        break;
    case BOT_WINS:
        robotObey = true;
        DoCelebrate(GameMoveComplete);
        break;
    default:
        break;
        
    }
    
}

void SimonSays(int instruction)
{
    int obey = random(0,5);
    SetMouth(ROBOTMOUTH_HAPPY);

    if(obey == 0)
    {
        instruction  = (instruction + random(1,SIMON_MAX)) % SIMON_MAX;
        robotObey = false;
    }
    else
    {
        robotObey = true;
    }

    robotState = ROBOT_BUSY;

    GameMove(instruction);
    
}

void NoSimon(int instruction)
{
    int obey = random(0,5);

    SetMouth(ROBOTMOUTH_HAPPY);

    if(obey == 0)
    {
       GameMove(instruction);
       robotObey = false;
       robotState = ROBOT_BUSY;
    }
    else
    {       
        robotObey = true;
        robotState = ROBOT_IDLE;
    }
    
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
    if(robotObey)
    {
        robotState = ROBOT_IDLE;
    }
    else
    {
        robotState = ROBOT_WRONG;
        SetMouth(ROBOTMOUTH_CONFUSE);
    }
}


