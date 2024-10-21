#ifndef CONFIG_H
#define CONFIG_H

#include <ESP32Servo.h>

//===============================eeprom asign===========================
//
//  m1 open =>  0
//  m1 close => 4
//
//  m2 open =>  8
//  m2 close => 12
//
//  m3 0 => 16
//  m3 1 => 20
//  m3 2 => 24
//  m3 3 => 28
//  m3 4 => 32
//  m3 5 => 36
//  m3 6 => 40


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
extern Servo M1Servo;
extern Servo M2Servo;
extern Servo M3Servo;

extern int defaultServo1;
extern int defaultServo2;
extern int defaultServo3;

extern int m1open;
extern int m1close;
extern int m2open;
extern int m2close;

extern int m3pos0;
extern int m3pos1;
extern int m3pos2;
extern int m3pos3;
extern int m3pos4;
extern int m3pos5; 
extern int m3pos6;

//=================================hardware===================================

#endif // CONFIG_H
