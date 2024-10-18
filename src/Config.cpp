#include "Config.h"

const char *ssid = "BELLY RICE GRAIN CLASSIFICATION MACHINE";
const char *password = "12345678";
// Initialize variables
unsigned long startTime = 0;
unsigned long pauseTime = 0;
unsigned long elapsedTime = 0;
bool isStarted = false;
bool isPaused = false;
bool isManualMode = true;
bool isSelected = false;

bool isFirsttime = true;

int positionCount[7] = {0};
int currentvalue = 0;

unsigned long lastScoopTime = 0;
unsigned long scoopInterval = 1000; // Time for scoop
unsigned long lastServoTime = 0;
unsigned long servoInterval = 2000; // Time for servo

// Initialize servo objects
Servo myservo1;
Servo myservo2;
Servo myservo3;

int defaultServo1 = 0;
int defaultServo2 = 180;
int defaultServo3 = 0;
