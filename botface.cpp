
#include <Wire.h>
#include "eyes.h"
#include "botface.h"

#define UPDATE_COUNT_MOVE_EYES  3
#define UPDATE_COUNT_BLINK      1

#define MAX_PENDING_ACTIONS 6

typedef enum
{
    EYES_MOVE,
    EYES_BLINKOPEN,
    EYES_BLINKCLOSE,
    EYES_SLEEP,
    EYES_WAKE,
    EYES_PAUSE,
}EYE_ACTION;


typedef enum
{
    EYE_R=0,
    EYE_L,
}EYE_DIR;

typedef struct
{
    int current;
    int target;
    EYE_DIR dir;
    
    
}EYE_MOVEMENT;

typedef struct
{
    EYE_ACTION action;
    EYE_MOVEMENT left;
    EYE_MOVEMENT right;
    int lid;
    bool blinkL;
    bool blinkR;
    int updateCount;
    int pauseCount;
    bool isInstruction;
    void (*callback) (void);

}EYES;


typedef struct
{
    EYE_COMMAND action;
    int leftTarget;
    int rightTarget;
    int pause;
    void (*callback)(void);
}EYE_PENDING;    


static EYES eyes;
static EYE_PENDING actionQueue[MAX_PENDING_ACTIONS];
static int numPendingActions = 0;
static int nextPendingAction = 0;
static int FindNextEyePosition(int current, int target, EYE_DIR dir);
static ROBOT_MOUTH nextMouth;

static void GetNextQueuedEyeAction(void);
static EYE_DIR GetShortestWay(int current, int target);
static void GetRandomEyeAction(void);
static void GetNextEyeAction(void);
static void ClearBotFace(void);
static void ShowLeftEye(int eyeIndex);
static void ShowRightEye(int eyeIndex);
static void CloseLeftEye(byte eyeIndex, byte blinkDepth);
static void CloseRightEye(byte eyeIndex, byte blinkDepth);
static void OpenLeftEye(byte eyeIndex, byte blinkDepth);
static void OpenRightEye(byte eyeIndex, byte blinkDepth);
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

    eyes.left.current = 12;
    eyes.right.current = 12;
    eyes.lid = 0;

    GetNextEyeAction();
}

void SetMouth(ROBOT_MOUTH mouth)
{
    nextMouth = mouth;
}


void UpdateEyes(void)
{

    eyes.updateCount++;

    switch(eyes.action)
    {
    case EYES_MOVE:

        if(eyes.updateCount >= UPDATE_COUNT_MOVE_EYES)
        {
            eyes.left.current = FindNextEyePosition(eyes.left.current, eyes.left.target, eyes.left.dir);
            eyes.right.current = FindNextEyePosition(eyes.right.current, eyes.right.target, eyes.right.dir);

            ShowLeftEye(eyes.left.current);
            ShowRightEye(eyes.right.current);
        
            if((eyes.left.current == eyes.left.target) && (eyes.right.current == eyes.right.target))
            {
                eyes.action = EYES_PAUSE;        
            }

            eyes.updateCount = 0;
        }
        break;

    case EYES_BLINKOPEN:

        if(eyes.updateCount >= UPDATE_COUNT_BLINK)
        {
            eyes.lid--;

            if(eyes.blinkL)
            {
                OpenLeftEye(eyes.left.current, eyes.lid);
            }
            if(eyes.blinkR)
            {
                OpenRightEye(eyes.right.current, eyes.lid);
            }
            if(eyes.lid == 0)
            {
                eyes.action = EYES_PAUSE;
            }
            eyes.updateCount = 0;
        }
        break;

    case EYES_BLINKCLOSE:


        if(eyes.updateCount >= UPDATE_COUNT_BLINK)
        {
            eyes.lid++;
            
            if(eyes.blinkL)
            {
                CloseLeftEye(eyes.left.current, eyes.lid);
            }
            if(eyes.blinkR)
            {
                CloseRightEye(eyes.right.current, eyes.lid);
            }
            if(eyes.lid == ((EYE_PAGES * 8) -1))
            {
                eyes.action = EYES_BLINKOPEN;
            }
            eyes.updateCount = 0;
        }
        break;

     case EYES_SLEEP:

        if(eyes.lid < ((EYE_PAGES*8) - 1))
        {
                    
            eyes.lid++;
            CloseLeftEye(eyes.left.current, eyes.lid);
            CloseRightEye(eyes.right.current, eyes.lid);
        }
        else
        {
            eyes.action = EYES_PAUSE;
        }
        break;

     case EYES_WAKE:

        if(eyes.lid == 0)
        {
            GetNextEyeAction();
        }
        else
        {
            eyes.lid--;
            OpenLeftEye(eyes.left.current, eyes.lid);
            OpenRightEye(eyes.right.current, eyes.lid);
        }
        
     case EYES_PAUSE:

           if(nextMouth != ROBOTMOUTH_NONE)
           {
                ShowMouth(nextMouth);
                nextMouth = ROBOTMOUTH_NONE;
           }

           if(eyes.updateCount >= eyes.pauseCount || (!eyes.isInstruction  && (numPendingActions > 0)))
           {
                GetNextEyeAction();
           }
           break;
    }

}

bool QueueEyeAction(EYE_COMMAND action, int leftTarget, int rightTarget, int pause, void (*callback)(void))
{
    int q;

    if(numPendingActions == MAX_PENDING_ACTIONS)
    {
        return false;
    }
    else
    {
        q = (nextPendingAction + numPendingActions) % MAX_PENDING_ACTIONS;
        actionQueue[q].action = action;
        actionQueue[q].leftTarget = leftTarget;
        actionQueue[q].rightTarget = rightTarget;
        actionQueue[q].pause = pause;
        actionQueue[q].callback = callback;

        numPendingActions++;
    }         

    return true;
}


static int FindNextEyePosition(int current, int target, EYE_DIR dir)
{

    if(current == target)
    {
        return current;
    }
    
    switch (dir)
    {
    case EYE_L:

        return (current + (NUM_EYES - 1)) % NUM_EYES;
        break;

    case EYE_R:

        return (current + 1) % NUM_EYES;
        break;
    default:
        return current;
    }
}

static void GetNextEyeAction(void)
{

    if(eyes.isInstruction && eyes.callback)
    {
        (*eyes.callback)();
        eyes.callback = NULL;
    }
    if( numPendingActions == 0)
    {
        GetRandomEyeAction();
    }
    else
    {
        GetNextQueuedEyeAction();
    }
    
}

static void GetNextQueuedEyeAction(void)
{
    
    Serial.println("Get Queued action");
    
    switch(actionQueue[nextPendingAction].action)
    {
    case EYE_COMMAND_LOOK:

        eyes.left.target = actionQueue[nextPendingAction].leftTarget;
        eyes.left.dir = GetShortestWay(eyes.left.current, eyes.left.target);
        eyes.right.target = actionQueue[nextPendingAction].rightTarget;
        eyes.right.dir = GetShortestWay(eyes.right.current, eyes.right.target);
        eyes.action = EYES_MOVE;
        eyes.pauseCount = actionQueue[nextPendingAction].pause;
        break;

    case EYE_COMMAND_BLINK:

        eyes.action = EYES_BLINKCLOSE;
        eyes.pauseCount = actionQueue[nextPendingAction].pause;
        break;

    case EYE_COMMAND_PAUSE:

        eyes.action = EYES_PAUSE;
        eyes.pauseCount = actionQueue[nextPendingAction].pause;
        break;

     case EYE_COMMAND_SLEEP:

        eyes.action = EYES_SLEEP;
        break;

    case EYE_COMMAND_WAKE:
    
        eyes.action = EYES_WAKE;
        break;
    }

    eyes.isInstruction = true;
    eyes.updateCount = 0;
    eyes.callback = actionQueue[nextPendingAction].callback;
    numPendingActions--;
    nextPendingAction = (nextPendingAction + 1) % MAX_PENDING_ACTIONS;
    
}

static EYE_DIR GetShortestWay(int current, int target)
{
    if(current < target)
    {
        if(target - current < NUM_EYES/2)
        {
            return EYE_R;
        }
        return EYE_L;
    }
    else
    {
        if(current-target < NUM_EYES/2)
        {
            return EYE_L;
        }
        return EYE_R;
    }
}

static void GetRandomEyeAction(void)
{

    int randAction;
    int randMovement;
    int randBlink;

    randAction = (int)random(0,12);
    eyes.pauseCount = (int)random(50,300);   
   

    if(randAction < 5)
    {
        eyes.action = EYES_MOVE;

        if(randAction < 4)
        {
          randMovement = (int)random(1,10);  
        }
        else
        {
           randMovement = (int)random(1,20); 
        }
        
        eyes.left.target = (eyes.left.current + randMovement) % NUM_EYES;
        eyes.left.dir = EYE_R;
        eyes.right.target = (eyes.right.current + randMovement) % NUM_EYES;    
        eyes.right.dir = EYE_R;
    }
    else if(randAction < 10)
    {
        eyes.action = EYES_MOVE;

        if(randAction < 9)
        {
          randMovement = (int)random(1,10);  
        }
        else
        {
           randMovement = (int)random(1,20); 
        }
        eyes.left.target = ((eyes.left.current + NUM_EYES) - randMovement) % NUM_EYES;
        eyes.left.dir = EYE_L;
        eyes.right.target = ((eyes.right.current + NUM_EYES) - randMovement) % NUM_EYES;
        eyes.right.dir = EYE_L;
    }
    else
    {
        eyes.action = EYES_BLINKCLOSE;
        randBlink = (int) random(1,20);
        if(randBlink < 18)
        {
            eyes.blinkL = true;
            eyes.blinkR = true;
        }
        else if(randBlink < 19)
        {
            eyes.blinkL = false;
            eyes.blinkR = true;
            
        }
        else
        {
            eyes.blinkL = true;
            eyes.blinkR = false;      
        } 

    }

    eyes.updateCount = 0;
    eyes.isInstruction = false;
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
            Wire.write(eyePics[eyeIndex][i]);
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
            Wire.write(eyePics[eyeIndex][i]);
            i++;
        }
        i--;
        Wire.endTransmission();
    }


}


static void CloseLeftEye(byte eyeIndex, byte blinkDepth)
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

    StartTransfer(LEFT_EYE_START_COLUMN,LEFT_EYE_END_COLUMN, blinkPage, blinkPage);

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=LEFT_EYE_START_COLUMN; j< LEFT_EYE_MID; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=LEFT_EYE_MID; j< LEFT_EYE_END_COLUMN + 1; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();
}

static void CloseRightEye(byte eyeIndex, byte blinkDepth)
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

    StartTransfer(RIGHT_EYE_START_COLUMN,RIGHT_EYE_END_COLUMN, blinkPage, blinkPage);

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=RIGHT_EYE_START_COLUMN; j< RIGHT_EYE_MID; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();

    Wire.beginTransmission(BOTFACE_ADDRESS);
    Wire.write(BOTFACE_CONTROL_DATA_STREAM);

    for(j=RIGHT_EYE_MID; j< RIGHT_EYE_END_COLUMN + 1; j++)
    {
        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;
    }

    Wire.endTransmission();

}

static void OpenLeftEye(byte eyeIndex, byte blinkDepth)
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


    StartTransfer(LEFT_EYE_START_COLUMN,LEFT_EYE_END_COLUMN, blinkPage, blinkPage);


    while(eyeByte < EYE_BYTES)
    {

        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;

        Wire.endTransmission();
    }

}

static void OpenRightEye(byte eyeIndex, byte blinkDepth)
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

    


    StartTransfer(RIGHT_EYE_START_COLUMN,RIGHT_EYE_END_COLUMN,blinkPage, blinkPage);

    while(eyeByte < EYE_BYTES)
    {
        Wire.beginTransmission(BOTFACE_ADDRESS);
        Wire.write(BOTFACE_CONTROL_DATA_STREAM);

        blinkedEye = (closedeye[eyeByte] & mask) | eyePics[eyeIndex][eyeByte];
        Wire.write(blinkedEye);
        eyeByte += EYE_PAGES;

        Wire.endTransmission(); 
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
