#include "HWFunction.h"
#include "Config.h" // Include your Config.h to access shared variables

String formatTime(unsigned long milliseconds)
{
  unsigned long totalSeconds = milliseconds / 1000;
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}

int calculateTotalCount()
{
  int total = 0;
  for (int i = 0; i < 7; i++)
  {
    total += positionCount[i];
  }
  return total;
}
void timepause()
{
  if (!isPaused)
  {
    isPaused = true;
    pauseTime = time_now; // Store the pause start time
  }
}

void start()
{
  if (isPaused)
  {
    isPaused = false;
    startTime += time_now - pauseTime; // Adjust startTime to account for the paused duration
  }
  else
  {
    startTime = time_now; // Reset startTime if not previously paused
  }
}

int readAIValue()
{
  int value = 0;

  // Read the pins and shift the bits accordingly
  for (int i = 0; i < 3; i++)
  {
    int pinValue = digitalRead(AIDATAIN[i]);
    value |= (pinValue << i); // Shift the pinValue by i bits and OR it to the value
  }

  return value; // Return the decimal value
}

const uint8_t SEG_ONE = 0x06;  // Segment pattern for "1"
const uint8_t SEG_ZERO = 0x3F; // Segment pattern for "0"
void displayBinary(int num) {
  // Limit number to 3 bits for display (000 to 111)
  num = num & 0x7; // Mask to keep only 3 bits (0b0111)

  // Create an array to hold each binary digit as "1" or "0"
  uint8_t data[] = {
    0x00,                              // First digit is blank
    (num & 0x4) ? SEG_ONE : SEG_ZERO,  // Second digit: 4's place
    (num & 0x2) ? SEG_ONE : SEG_ZERO,  // Third digit: 2's place
    (num & 0x1) ? SEG_ONE : SEG_ZERO   // Fourth digit: 1's place
  };

  // Display the binary on the screen
  display.setSegments(data);
}
