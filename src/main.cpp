#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "Server.h"
#include "Config.h"
#include "HWFunction.h"
#include "M1.h"
#include "M2.h"
#include "M3.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable detector
  // Initialize EEPROM with size enough to store your data
  EEPROM.begin(44); // 4 bytes per int, total of 3 integers

  pinMode(irReceiverPin, INPUT_PULLUP);
  pinMode(ledIRPin, OUTPUT);
  pinMode(AIoutPin, OUTPUT);
  for (int i = 0; i < 3; i++)
  {
    pinMode(pinIn[i], INPUT_PULLUP);
  }
  pinMode(ledStartPin, OUTPUT);
  pinMode(ledStopPin, OUTPUT);

  M1Servo.attach(M1ServoPIN);
  M2Servo.attach(M2ServoPIN);
  M3Servo.attach(M3ServoPIN);

  Serial.begin(115200);

  // Initialize Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.println("Hosted WiFi hotspot");
  Serial.println(WiFi.softAPIP());

  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  // Define web server routes
  server.on("/", indexfile);
  server.on("/style.css", HTTP_GET, style);
  server.on("/script.js", script);
  server.on("/data", HTTP_GET, generateJSON);
  server.on("/pause", HTTP_GET, Pause);
  server.on("/reset", HTTP_GET, reset);
  server.on("/home", HTTP_GET, Home);

  server.on("/start/manual", HTTP_GET, manualmode);
  server.on("/start/random", HTTP_GET, randommode);
  server.on("/selected", HTTP_GET, selected);
  server.on("/getcalibation", HTTP_GET, getCalibationValueJson);
  server.on("/calibation", HTTP_GET, calibation);
  server.on("/testcalibation", HTTP_GET, testCalibation);

  // Start the server
  // server.begin();
  //
  M1Read(m1open, m1close);
  M2Read(m2open, m2close);
}

int m1delay = 1000;
int m2delay = 1000;
int m3delay = 1000;
unsigned long time_now = 0;
unsigned long time_period_m1 = 0;
unsigned long time_period_m2 = 0;
unsigned long time_period_m3 = 0;

bool ism1open = false;
void loop()
{
  time_now = millis();

  switch (CurrentState)
  {
  case OFF:
    digitalWrite(AIoutPin, LOW); // Ensure AI output is off
    break;

  case PAUSE:
    // No actions during pause state
    break;

  case M1:
    M2Close();
    digitalWrite(AIoutPin, LOW); // Turn off AI output in M1
    if (digitalRead(irReceiverPin) == HIGH)
    {
      CurrentState = AI;            // Transition to AI state
      M1Close();                    // Call M1 close function
      digitalWrite(AIoutPin, HIGH); // Activate AI output
    }
    else if (time_now - time_period_m1 > m1delay)
    {
      // Toggle the state of M1 based on its current state
      if (!ism1open)
      {
        M1Open();        // Open M1
        ism1open = true; // Update state to open
      }
      else
      {
        M1Close();        // Close M1
        ism1open = false; // Update state to closed
      }

      // Optionally, you may want to reset time_period here
      time_period_m1 = time_now; // Reset the time_period to the current time
    }
    break;

  case AI:

    if (CurrentMode == Manual)
    {
      aivalue = readAIValue(); // Get AI value
    }
    else
    {
      aivalue = random(0, 8); // Generate a random integer between 0 and 7
    }
    if (aivalue != 7) // Condition to change state
    {
      CurrentState = M3;         // Transition to M3 state if condition met
      time_period_m3 = time_now; // Reset the time_period to the current time
    }
    break;

  case M3:
    M3Move(aivalue); // Call M3 movement function with the current AI value
    if (time_now - time_period_m3 > m3delay)
    {
      CurrentState = M2;
      time_period_m3 = time_now;
      time_period_m2 = time_now; // Reset the time_period to the current time
    }

    break;

  case M2:
    M2Open();
    if (time_now - time_period_m2 > m2delay)
    {

      CurrentState = M1;
      time_period_m2 = time_now;
      time_period_m1 = time_now;
    }
    // Add any specific logic for M2 here if needed
    break;

  default:
    // Default case to handle unexpected states
    break;
  }
}
