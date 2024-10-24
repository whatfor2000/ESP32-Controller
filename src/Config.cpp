#include "Config.h"

const char *ssid = "BELLY RICE GRAIN CLASSIFICATION MACHINE";
const char *password = "12345678";
// Initialize variables
unsigned long startTime = 0;
unsigned long pauseTime = 0;
unsigned long time_now = 0;
bool isStarted = false;
bool isPaused = false;

bool isFirsttime = true;

int positionCount[7] = {0};
int currentvalue = 0;


// Initialize servo objects
Servo M1Servo;
Servo M2Servo;
Servo M3Servo;

int defaultServo1 = 0;
int defaultServo2 = 180;
int defaultServo3 = 0;

int m1open = 0;
int m1close = 0;
int m2open = 0;
int m2close = 0;

int m3pos0 = 0;
int m3pos1 = 0;
int m3pos2 = 0;
int m3pos3 = 0;
int m3pos4 = 0;
int m3pos5 = 0; 
int m3pos6 = 0;   

State CurrentState = OFF;
Mode CurrentMode = Random;

int aivalue = 7;