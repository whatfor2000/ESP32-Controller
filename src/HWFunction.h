#ifndef HWFUNCTION_H
#define HWFUNCTION_H

#include <Arduino.h>

// Function prototypes
String formatTime(unsigned long milliseconds);
int calculateTotalCount();
void timepause();
void start();
int readAIValue();

#endif // HWFUNCTION_H
