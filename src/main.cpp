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

void setup()
{

  // Initialize EEPROM with size enough to store your data
  EEPROM.begin(44); // 4 bytes per int, total of 3 integers

  pinMode(irReceiverPin, INPUT_PULLUP);
  pinMode(ledIRPin, OUTPUT);
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
  server.begin();
}

void loop()
{

}
