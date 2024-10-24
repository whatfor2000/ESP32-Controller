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
  if (isStarted) // Only allow pausing if the system has started
  {
    if (isPaused) // If already paused, resume the system
    {
      isPaused = false;
      startTime += millis() - pauseTime; // Adjust start time to continue from where it was paused
    }
    else // If running, pause the system
    {
      isPaused = true;
      pauseTime = millis(); // Record the time when paused
    }
  }
}
void start()
{
  if (isStarted)
  {
    if (isPaused) // If currently paused, we want to resume
    {
      isPaused = false;
      startTime += millis() - pauseTime; // Adjust start time to continue counting from where it paused
    }
    else // If running, we want to pause
    {
      isPaused = true;
      pauseTime = millis(); // Record the time when paused
    }
  }
  else // Starting for the first time
  {
    isStarted = true;
    isPaused = false;

    if (isFirsttime)
    {
      startTime = millis(); // Start fresh
      isFirsttime = false; // Set the first time flag to false after starting
    }
    else
    {
      startTime = millis() - time_now; // Continue from where it was paused
    }
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
