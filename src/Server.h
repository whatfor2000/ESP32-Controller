#ifndef SERVER_H
#define SERVER_H

#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

extern AsyncWebServer server;

void handleRoot(AsyncWebServerRequest *request);
void generateJSON(AsyncWebServerRequest *request);
void generateServoSettingsJSON(AsyncWebServerRequest *request);
void setServo(AsyncWebServerRequest *request);
void Home(AsyncWebServerRequest *request);
void Pause(AsyncWebServerRequest *request);
void reset(AsyncWebServerRequest *request);
void manualmode(AsyncWebServerRequest *request);
void randommode(AsyncWebServerRequest *request);
void selected(AsyncWebServerRequest *request);
void readServoValues(int &servo1, int &servo2, int &servo3);


#endif