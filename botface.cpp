
#include <Wire.h>
#include "botface.h"
#include "eyes.h"
#include "mouth.h"

#define UPDATE_COUNT_MOVE_EYES  3
#define UPDATE_COUNT_BLINK      1

typedef struct
{
    int pos;
    int lid;
}EYE_STATE;

typedef enum
{
    EYES_MOVELEFT,
    EYES_MOVERIGHT,
    EYES_BLINKOPEN,
    EYES_BLINKCLOSE,
    EYES_PAUSE,
}EYE_ACTION;

typedef struct
{
    EYE_ACTION action;
    EYE_STATE current;
    EYE_STATE target;
    int updateCount; 
    int updateCountTarget;  
    int pause;

}EYES;


static EYES eye;


static void GetNextEyeAction(void);
static void ClearBotFace(void);
static void ShowLeftEye(int eyeIndex);
static void ShowRightEye(int eyeIndex);
static void CloseEye(byte eyeIndex, byte blinkDepth, bool left, bool right);
static void OpenEye(byte eyeIndex, byte blinkDepth, bool left, bool right);
static void ShowMouth(ROBOT_MOUTH mouth);
static void ScreenInit(void);
static void StartTransfer(byte startCol, byte endCol, byte startPage, byte endPage);


void InitBotFace(void)
{


    ScreenInit();
    ClearBotFace();
    Wire.setClock(1000000L);

    ShowLeftEye(12);
    ShowRightEye(12);
    ShowMouth(ROBOTMOUTH_HAPPY);

    eye.current.pos = 12;
    eye.current.lid = 0;

    GetNextEyeAction();
}


void UpdateEyes(void)
{

    eye.updateCount++;

    if(eye.updateCount < eye.updateCountTarget)
    {
        return;
    }


    switch(eye.action)
    {
        case EYES_MOVELEFT:

            eye.current.pos = (eye.current.pos + 1) % NUM_EYES;

            ShowLeftEye(eye.current.pos);
            ShowRightEye(eye.current.pos);

            if(eye.current.pos == eye.target.pos)
            {
                eye.action = EYES_PAUSE;
                eye.updateCountTarget = eye.pause;
            }

            eye.updateCount = 0;

            break;

        case EYES_MOVERIGHT:

            eye.current.pos = (eye.current.pos  + (NUM_EYES - 1)) % NUM_EYES;

            ShowLeftEye(eye.current.pos);
            ShowRightEye(eye.current.pos);

            if(eye.current.pos == eye.target.pos)
            {
                eye.action = EYES_PAUSE;
                eye.updateCountTarget = eye.pause;
            }

            eye.updateCount = 0;
            break;

        case EYES_BLINKOPEN:

            eye.current.lid = eye.current.lid - 1;

            OpenEye(eye.current.pos, eye.current.lid, true, true);

            if(eye.current.lid == 0)
            {
                eye.action = EYES_PAUSE;
                eye.updateCountTarget = eye.pause;
            }

            eye.updateCount = 0;
            break;

        case EYES_BLINKCLOSE:

            eye.current.lid = eye.current.lid + 1;

            CloseEye(eye.current.pos, eye.current.lid, true, true);

            if(eye.current.lid ==  (EYE_PAGES * 8 -1))
            {
                eye.action = EYES_BLINKOPEN;
            }

            eye.updateCount = 0;
            break;

        case EYES_PAUSE:

            GetNextEyeAction();
            break;
    }

}


static void GetNextEyeAction(void)
{

    int randAction;
    int randMovement;


    randAction = (int)random(0,12);

    eye.pause = (int)random(100,500);


    if(randAction < 4)
    {
        eye.action = EYES_MOVELEFT;
        eye.updateCountTarget = UPDATE_COUNT_MOVE_EYES;
        randMovement = (int)random(1,10);
        eye.target.pos = (eye.current.pos + randMovement) % NUM_EYES;
        eye.updateCount = 0;
    }
    else if(randAction < 5)
    {
        eye.action = EYES_MOVELEFT;
        eye.updateCountTarget = UPDATE_COUNT_MOVE_EYES;
        randMovement = (int)random(1,20);
        eye.target.pos = (eye.current.pos + randMovement) % NUM_EYES;
    }
    else if(randAction < 9)
    {
        eye.action = EYES_MOVERIGHT;
        eye.updateCountTarget = UPDATE_COUNT_MOVE_EYES;
        randMovement = (int)random(1,10);
        eye.target.pos = ((eye.current.pos + NUM_EYES) - randMovement) % NUM_EYES;
        eye.updateCount = 0;
    }
    else if(randAction < 10)
    {
        eye.action = EYES_MOVERIGHT;
        eye.updateCountTarget = UPDATE_COUNT_MOVE_EYES;
        randMovement = (int)random(1,20);
        eye.target.pos = ((eye.current.pos + NUM_EYES) - randMovement) % NUM_EYES;
        eye.updateCount = 0;
    }
    else
    {
        eye.action = EYES_BLINKCLOSE;
        eye.updateCountTarget = UPDATE_COUNT_BLINK;
        eye.updateCount = 0;
        eye.pause = (int)random(50,100);
    }
}


static void ClearBotFace(void)
{

    StartTransfer(0, BOTFACE_LAST_COL, 0, BOTFACE_LAST_PAGE);

    for(int i=0; i< BOTFACE_TOTAL_BYTES; i++)
    {

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(int j=0; j < 16; j++)
        {
            Wire.write(0);
            i++;
        }
        i--;
        Wire.endTransmission();
    }

}





static void ShowLeftEye(int eyeIndex)
{

    StartTransfer(LEFT_EYE_START_COLUMN,LEFT_EYE_END_COLUMN, EYE_START_PAGE, EYE_END_PAGE);

    for(int i=0; i<EYE_BYTES; i++)
    {

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(int j=0; j < EYE_PAGES; j++)
        {
            Wire.write(eyes[eyeIndex][i]);
            i++;
        }
        i--;
        Wire.endTransmission();
    }
}

static void ShowRightEye(int eyeIndex)
{

    StartTransfer(RIGHT_EYE_START_COLUMN,RIGHT_EYE_END_COLUMN, EYE_START_PAGE, EYE_END_PAGE);

    for(int i=0; i<EYE_BYTES; i++)
    {

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(int j=0; j < EYE_PAGES; j++)
        {
            Wire.write(eyes[eyeIndex][i]);
            i++;
        }
        i--;
        Wire.endTransmission();
    }


}


static void CloseEye(byte eyeIndex, byte blinkDepth, bool left, bool right)
{

    byte blinkPage;
    byte blinkBits;
    byte mask = 0;
    byte blinkedEye;

    int maskBit, eyeByte, j;

    if((blinkDepth == 0) | (blinkDepth > (EYE_PAGES * 8)))
    {
        return;
    }


    blinkPage = (blinkDepth-1) >> 3;

    blinkBits = (blinkDepth - (8 * blinkPage));

    for(maskBit=0; maskBit< blinkBits; maskBit++)
    {
        mask |= (1<<maskBit);
    }

    eyeByte = blinkPage;

    if(left)
    {

        StartTransfer(LEFT_EYE_START_COLUMN,LEFT_EYE_END_COLUMN, blinkPage, blinkPage);

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(j=LEFT_EYE_START_COLUMN; j< LEFT_EYE_MID; j++)
        {
            blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
            Wire.write(blinkedEye);
            eyeByte += EYE_PAGES;
        }

        Wire.endTransmission();

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(j=LEFT_EYE_MID; j< LEFT_EYE_END_COLUMN + 1; j++)
        {
            blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
            Wire.write(blinkedEye);
            eyeByte += EYE_PAGES;
        }

        Wire.endTransmission();

    }

    eyeByte = blinkPage;

    if(right)

        StartTransfer(RIGHT_EYE_START_COLUMN,RIGHT_EYE_END_COLUMN, blinkPage, blinkPage);

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=RIGHT_EYE_START_COLUMN; j< RIGHT_EYE_MID; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=RIGHT_EYE_MID; j< RIGHT_EYE_END_COLUMN + 1; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();

}

static void OpenEye(byte eyeIndex, byte blinkDepth, bool left, bool right)
{

    byte blinkPage;
    byte blinkBits;
    byte mask = 0;
    byte blinkedEye;

    int maskBit, eyeByte;

    if((blinkDepth >= (EYE_PAGES * 8)))
    {
        return;
    }


    blinkPage = blinkDepth >> 3;

    blinkBits = blinkDepth & 7;

    for(maskBit=0; maskBit< blinkBits; maskBit++)
    {
        mask |= (1<<maskBit);
    }

    eyeByte = blinkPage;

    if(left)
    {

        StartTransfer(LEFT_EYE_START_COLUMN,LEFT_EYE_END_COLUMN, blinkPage, blinkPage);


        while(eyeByte < EYE_BYTES)
        {

            Wire.beginTransmission(BOTFACE_ADDRESS);
            Wire.write(BOTFACE_CONTROL_DATA_STREAM);

            blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
            Wire.write(blinkedEye);
            eyeByte += EYE_PAGES;

            Wire.endTransmission();
        }

    }

    eyeByte = blinkPage;

    if(right)
    {
        StartTransfer(RIGHT_EYE_START_COLUMN,RIGHT_EYE_END_COLUMN,blinkPage, blinkPage);

        while(eyeByte < EYE_BYTES)
        {
            Wire.beginTransmission(BOTFACE_ADDRESS);
            Wire.write(BOTFACE_CONTROL_DATA_STREAM);

            blinkedEye = (closedeye[eyeByte] & mask) | eyes[eyeIndex][eyeByte];
            Wire.write(blinkedEye);
            eyeByte += EYE_PAGES;

            Wire.endTransmission(); 
        }

    }
}

static void ShowMouth(ROBOT_MOUTH mouth)
{

    StartTransfer(MOUTH_START_COLUMN, MOUTH_END_COLUMN, MOUTH_START_PAGE, MOUTH_END_PAGE);

    for(int i=0; i< MOUTH_BYTES; i++)
    {
        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        for(int j=0; j<MOUTH_PAGES; j++)
        {
            Wire.write(mouths[mouth][i]);
            i++;
        }
        i--;
        Wire.endTransmission();
    }
}


static void ScreenInit(void)
{

    Wire.begin();

    TWBR=12;

    Wire.beginTransmission(BOTFACE_ADDRESS);

    Wire.write(BOTFACE_CONTROL_CMD_STREAM);
    Wire.write(BOTFACE_CMD_DISPLAY_OFF);
    Wire.write(BOTFACE_CMD_SET_MUX_RATIO);
    Wire.write(BOTFACE_NUM_ROWS-1); 
    Wire.write(BOTFACE_CMD_SET_DISPLAY_OFFSET);
    Wire.write(0x00);    
    Wire.write(BOTFACE_CMD_DISPLAY_START_MASK | 0x00);


    //Orientation
    Wire.write(BOTFACE_CMD_SET_SEGMENT_REMAP);
    Wire.write(BOTFACE_CMD_SET_COM_SCAN_MODE);

    // Default - alternate COM pin map
    Wire.write(BOTFACE_CMD_SET_COM_PIN_MAP);
    Wire.write(0x12);

    Wire.write(BOTFACE_CMD_SET_CONTRAST_CTRL);
    Wire.write(0x7F);

    // Set display to enable rendering from GDDRAM (Graphic Display Data RAM)    
    Wire.write(BOTFACE_CMD_DISPLAY_RAM);
    Wire.write(BOTFACE_CMD_DISPLAY_NORMAL);

    // Default oscillator clock
    Wire.write(BOTFACE_CMD_SET_CLOCKDIV);
    Wire.write(0x80);

    // Enable the charge pump
    Wire.write(BOTFACE_CMD_SET_CHARGEPUMP);
    Wire.write(0x14);

    // Set precharge cycles to high cap type
    Wire.write(BOTFACE_CMD_SET_PRECHARGE);
    Wire.write(0x22);

    // Set the V_COMH deselect volatage to max
    Wire.write(BOTFACE_CMD_SET_VCOMH_DESELECT);
    Wire.write(0x30);

    // Horizonatal addressing mode - same as the KS108 GLCD
    Wire.write(BOTFACE_CMD_SET_MEMORY_ADDR_MODE);
    Wire.write(BOTFACE_CMD_VERTICAL_MODE);

    Wire.write(BOTFACE_CMD_DISPLAY_ON);

    Wire.endTransmission();


}

static void StartTransfer(byte startCol, byte endCol, byte startPage, byte endPage)
{
    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_CMD_STREAM);
    Wire.write(BOTFACE_CMD_SET_COLUMN_RANGE);
    Wire.write(startCol);
    Wire.write(endCol);
    Wire.write(BOTFACE_CMD_SET_PAGE_RANGE);
    Wire.write(startPage);
    Wire.write(endPage);
    Wire.endTransmission();

}
