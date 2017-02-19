#ifndef ROBOT_MOUTH_H
#define ROBOT_MOUTH_H


#define MOUTH_START_COLUMN    0x30
#define MOUTH_END_COLUMN      0x4F
#define MOUTH_START_PAGE      0x05
#define MOUTH_END_PAGE        0x06

#define MOUTH_COLUMNS         ((MOUTH_END_COLUMN - MOUTH_START_COLUMN) + 1)
#define MOUTH_PAGES           ((MOUTH_END_PAGE - MOUTH_START_PAGE) + 1)
#define MOUTH_BYTES           (MOUTH_COLUMNS * MOUTH_PAGES)

typedef enum{

  ROBOTMOUTH_HAPPY,
  ROBOTMOUTH_SAD,
  ROBOTMOUTH_CONFUSE,
  ROBOTMOUTH_SHOCK,
  ROBOTMOUTH_MAX
  
}ROBOT_MOUTH;

byte mouths[ROBOTMOUTH_MAX][MOUTH_BYTES] = 
{
  { 0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00,
    0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x04,
    0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x20,
    0x00, 0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40,
    0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x20,
    0x00, 0x20, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08,
    0x00, 0x04, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01,
    0x80, 0x00, 0x40, 0x00, 0x20, 0x00, 0x10, 0x00,
  },

  {
    0x00, 0x08, 0x00, 0x04, 0x00, 0x02, 0x00, 0x01,
    0x80, 0x00, 0x40, 0x00, 0x20, 0x00, 0x20, 0x00,
    0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00,
    0x04, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00,
    0x20, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00,
    0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08
  },
  
  {
    0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x01,
    0x00, 0x02, 0x00, 0x04, 0x00, 0x04, 0x00, 0x02,
    0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x01,
    0x00, 0x02, 0x00, 0x04, 0x00, 0x04, 0x00, 0x02,
    0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00,
    0x20, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x01 
  },

  {  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0x03, 0x60, 0x0c, 0x10, 0x10, 
    0x08, 0x20, 0x04, 0x40, 0x04, 0x40, 0x02, 0x80, 
    0x02, 0x80, 0x02, 0x80, 0x04, 0x40, 0x04, 0x40, 
    0x08, 0x20, 0x10, 0x10, 0x60, 0x0c, 0x80, 0x03, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  }
  
};





















#endif