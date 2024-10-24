#ifndef HWFUNCTION_H
#define HWFUNCTION_H

#include <Arduino.h>

// Function prototypes
void updateElapsedTime();
String formatTime(unsigned long milliseconds);
int calculateTotalCount();
void start();
int readAIValue();

#endif // HWFUNCTION_H
