#ifndef CONFIG_H
#define CONFIG_H

#include <ESP32Servo.h>

//=================================web===================================
// Wi-Fi Credentials
extern const char *ssid;
extern const char *password;

// Variables to track time, positions, and state
extern unsigned long startTime;
extern unsigned long pauseTime;
extern unsigned long elapsedTime;
extern bool isStarted;
extern bool isPaused;
extern bool isManualMode;
extern bool isSelected;

//=================================hardware===================================
extern bool isFirsttime;
const int pinTraffic = 19;
const int pinIn[] = {27, 26, 25}; // DIP switch pins
const int servoPin = 12;
const int servoPin2 = 13;
const int servoScoop = 19;
const int irReceiverPin = 5;

const int ledStartPin = 16; // Green LED for Start
const int ledStopPin = 17;  // Red LED for Stop
const int ledIRPin = 4;     // Yellow LED for IR sensor detection

const int numberOfPositions = 7;

// Servo positions array
extern int positionCount[7];
extern int currentvalue;

// Timing variables for millis()
extern unsigned long lastScoopTime;
extern unsigned long scoopInterval;
extern unsigned long lastServoTime;
extern unsigned long servoInterval;

// Create servo objects
extern Servo myservo1;
extern Servo myservo2;
extern Servo myservo3;

extern int defaultServo1;
extern int defaultServo2;
extern int defaultServo3;

//=================================hardware===================================

#endif // CONFIG_H
