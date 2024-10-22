#include "M2.h"
#include "Config.h"
#include <EEPROM.h>
void M2Open(){
    M2Servo.write(m2open);
}
void M2Close(){
    M2Servo.write(m2close);
}
void M2Read(int &open, int &close){

  int tempValue;
  EEPROM.get(8, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  { // Assuming servo range 0-180
    open = tempValue;
  }
  else
  {
    EEPROM.put(8, 0); // Store default value
  }
  EEPROM.get(12, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  { // Assuming servo range 0-180
    close = tempValue;
  }
  else
  {
    EEPROM.put(12, 0); // Store default value
  }
  EEPROM.commit();
}

void M2Set(int open, int close)
{
    EEPROM.put(8,open);
    EEPROM.put(12,close);
    EEPROM.commit();
}
