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
    int pinValue = digitalRead(pinIn[i]);
    value |= (pinValue << i); // Shift the pinValue by i bits and OR it to the value
  }

  return value; // Return the decimal value
}
