#include "Server.h"
#include "Config.h"
#include "M1.h"
#include "M2.h"
#include "M3.h"
#include <EEPROM.h>
#include "HWFunction.h"

AsyncWebServer server(80);
void indexfile(AsyncWebServerRequest *request)
{
  File file = SPIFFS.open("/index.html", "r");
  if (!file)
  {
    request->send(500, "text/plain", "File not found");
    return;
  }
  request->send(200, "text/html", file.readString()); // Serve the content of the file
  file.close();                                       // Close the file after serving
}

void style(AsyncWebServerRequest *request)
{
  File file = SPIFFS.open("/style.css", "r");
  if (!file)
  {
    request->send(500, "text/plain", "File not found");
    return;
  }
  request->send(200, "text/css", file.readString()); // Serve the content of the file
  file.close();                                      // Close the file after serving
}

void script(AsyncWebServerRequest *request)
{
  File file = SPIFFS.open("/script.js", "r");
  if (!file)
  {
    request->send(500, "text/plain", "File not found");
    return;
  }
  request->send(200, "text/html", file.readString()); // Serve the content of the file
  file.close();                                       // Close the file after serving
}

// Function for generating main JSON data
void generateJSON(AsyncWebServerRequest *request)
{
  String json = "{";
  json += "\"elapsedTime\":\"" + formatTime(elapsedTime) + "\",";
  json += "\"isStarted\":" + String(isStarted ? "true" : "false") + ",";
  json += "\"positionCount\":[";
  for (int i = 0; i < 7; i++)
  {
    json += String(positionCount[i]);
    if (i < 6)
      json += ",";
  }
  json += "],";
  json += "\"totalCount\":" + String(calculateTotalCount()) + ",";
  json += "\"currentValue\":" + String(currentvalue) + ",";
  json += "\"isManualMode\":" + String(isManualMode ? "true" : "false");
  json += "}";
  request->send(200, "application/json", json);
}

// New function for generating servo settings JSON
void getCalibationValueJson(AsyncWebServerRequest *request)
{
  M1Read(m1open, m1close);
  M2Read(m2open, m2close);
  M3Read(m3pos0, m3pos1, m3pos2, m3pos3, m3pos4, m3pos5, m3pos6);
  String json = "{";
  json += "\"m1open\":" + String(m1open) + ",";
  json += "\"m1close\":" + String(m1close) + ",";
  json += "\"m2open\":" + String(m2open) + ",";
  json += "\"m2close\":" + String(m2close) + ",";
  json += "\"m3pos0\":" + String(m3pos0) + ",";
  json += "\"m3pos1\":" + String(m3pos1) + ",";
  json += "\"m3pos2\":" + String(m3pos2) + ",";
  json += "\"m3pos3\":" + String(m3pos3) + ",";
  json += "\"m3pos4\":" + String(m3pos4) + ",";
  json += "\"m3pos5\":" + String(m3pos5) + ",";
  json += "\"m3pos6\":" + String(m3pos6);
  json += "}";
  request->send(200, "application/json", json);
}
void Pause(AsyncWebServerRequest *request)
{
  isStarted = false;
  isPaused = false;
  pauseTime = startTime;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void reset(AsyncWebServerRequest *request)
{
  isStarted = false;
  isPaused = false;
  elapsedTime = 0;
  for (int i = 0; i < 7; i++)
  {
    positionCount[i] = 0;
  }
  digitalWrite(ledStartPin, LOW);
  digitalWrite(ledStopPin, HIGH);
  // Home();
  M1Servo.write(defaultServo1);
  M2Servo.write(defaultServo2);
  ;
  M3Servo.write(defaultServo3);
  ;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void manualmode(AsyncWebServerRequest *request)
{
  isManualMode = true;
  start();
  isFirsttime = false;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void randommode(AsyncWebServerRequest *request)
{
  isManualMode = false;
  start();
  isFirsttime = false;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void selected(AsyncWebServerRequest *request)
{
  digitalWrite(ledIRPin, LOW);
  M2Servo.write(0);
  isSelected = true;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void calibation(AsyncWebServerRequest *request)
{
  // Check for the parameters and get their values
  if (request->hasParam("m1open") && request->hasParam("m1close") &&
      request->hasParam("m2open") && request->hasParam("m2close") &&
      request->hasParam("m3pos0") && request->hasParam("m3pos1") &&
      request->hasParam("m3pos2") && request->hasParam("m3pos3") &&
      request->hasParam("m3pos4") && request->hasParam("m3pos5") &&
      request->hasParam("m3pos6"))
  {

    // Retrieve and convert parameters
    int m1open = request->getParam("m1open")->value().toInt();
    int m1close = request->getParam("m1close")->value().toInt();
    int m2open = request->getParam("m2open")->value().toInt();
    int m2close = request->getParam("m2close")->value().toInt();
    int m3pos0 = request->getParam("m3pos0")->value().toInt();
    int m3pos1 = request->getParam("m3pos1")->value().toInt();
    int m3pos2 = request->getParam("m3pos2")->value().toInt();
    int m3pos3 = request->getParam("m3pos3")->value().toInt();
    int m3pos4 = request->getParam("m3pos4")->value().toInt();
    int m3pos5 = request->getParam("m3pos5")->value().toInt();
    int m3pos6 = request->getParam("m3pos6")->value().toInt();

    // Call your set functions
    Serial.println("Setting parameters...");
    M1Set(m1open, m1close);
    M2Set(m2open, m2close);
    M3Set(m3pos0, m3pos1, m3pos2, m3pos3, m3pos4, m3pos5, m3pos6);
  }
  else
  {
    // Handle missing parameters
    request->send(400, "text/plain", "Missing parameters");
    return;
  }

  // Redirect back to the main page
  request->redirect("/");
}
void testCalibation(AsyncWebServerRequest *reqest)
{
  String Servo = request->getParam("servo")->value();
  String position = request->getParam("position")->value();
}
void Home(AsyncWebServerRequest *request)
{
  isStarted = false;
  isPaused = false;
  M1Servo.write(defaultServo1);
  M2Servo.write(defaultServo2);
  ;
  M3Servo.write(defaultServo3);
  ;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}