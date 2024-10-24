#include "M3.h"
#include <EEPROM.h>
#include "Config.h"
#include <iostream>
#include <unordered_map>
void M3Move(int position)
{   
    // Read positions if necessary
    M3Read(m3pos0, m3pos1, m3pos2, m3pos3, m3pos4, m3pos5, m3pos6);
    // Store your position variables in an array
    int m3Positions[7] = {m3pos0, m3pos1, m3pos2, m3pos3, m3pos4, m3pos5, m3pos6};
    
    
    // Check if the position index is within the valid range (0-6)
    if (position >= 0 && position <= 6)
    {
        // Move the servo to the desired position
        M3Servo.write(m3Positions[position]);
        Serial.println("M3Servo moved to position: " + String(m3Positions[position]));
    }
    else
    {
        Serial.println("Invalid position value: " + String(position));
    }
}
void M3Read(int &POS0, int &POS1, int &POS2, int &POS3, int &POS4, int &POS5, int &POS6)
{
    int tempValue;

    EEPROM.get(16, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS0 = tempValue;
    }
    else
    {
        EEPROM.put(16, 0); // Store default value
    }

    EEPROM.get(20, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS1 = tempValue;
    }
    else
    {
        EEPROM.put(20, 0); // Store default value
    }

    EEPROM.get(24, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS2 = tempValue;
    }
    else
    {
        EEPROM.put(24, 0); // Store default value
    }

    EEPROM.get(28, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS3 = tempValue;
    }
    else
    {
        EEPROM.put(28, 0); // Store default value
    }

    EEPROM.get(32, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS4 = tempValue;
    }
    else
    {
        EEPROM.put(32, 0); // Store default value
    }

    EEPROM.get(36, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS5 = tempValue;
    }
    else
    {
        EEPROM.put(36, 0); // Store default value
    }

    EEPROM.get(40, tempValue);
    if (tempValue >= 0 && tempValue <= 180)
    { // Assuming servo range 0-180
        POS6 = tempValue;
    }
    else
    {
        EEPROM.put(40, 0); // Store default value
    }
    EEPROM.commit();
}

void M3Set(int POS0, int POS1, int POS2, int POS3, int POS4, int POS5, int POS6)
{
    EEPROM.put(16,POS0);
    EEPROM.put(20,POS1);
    EEPROM.put(24,POS2);
    EEPROM.put(28,POS3);
    EEPROM.put(32,POS4);
    EEPROM.put(36,POS5);
    EEPROM.put(40,POS6);
    EEPROM.commit();
}
