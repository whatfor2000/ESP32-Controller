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
extern unsigned long time_now;
extern bool isStarted;
extern bool isPaused;

//=================================hardware===================================
extern bool isFirsttime;
const int AIoutPin = 19;
const int pinIn[] = {2, 4, 5}; // DIP switch pins
const int M1ServoPIN = 25;
const int M2ServoPIN = 26;
const int M3ServoPIN = 27;
const int irReceiverPin = 15;

const int ledStartPin = 16; // Green LED for Start
const int ledStopPin = 17;  // Red LED for Stop
const int ledIRPin = 4;     // Yellow LED for IR sensor detection

const int numberOfPositions = 7;

// Servo positions array
extern int positionCount[7];
extern int currentvalue;


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

enum State
{
    OFF,
    PAUSE,
    M1,
    AI,
    M3,
    M2,
};
extern State CurrentState;
enum  Mode {
    Manual,
    Random
};
extern Mode CurrentMode;
extern int aivalue;
//=================================hardware===================================

#endif // CONFIG_H
