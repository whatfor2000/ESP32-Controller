#include "M1.h"
#include "Config.h"
#include <EEPROM.h>
void M1Open()
{
  M1Servo.write(m1open);
}
void M1Close()
{
  M1Servo.write(m1close);
}
void M1Read(int &open, int &close)
{

  int tempValue;
  EEPROM.get(0, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  { // Assuming servo range 0-180
    open = tempValue;
  }
  else
  {
    EEPROM.put(0, 0); // Store default value
  }
  EEPROM.get(4, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  { // Assuming servo range 0-180
    close = tempValue;
  }
  else
  {
    EEPROM.put(4, 0); // Store default value
  }
  EEPROM.commit();
}

void M1Set(int open, int close)
{ 
  EEPROM.put(0, open);
  EEPROM.put(4, close);
  EEPROM.commit();
}
