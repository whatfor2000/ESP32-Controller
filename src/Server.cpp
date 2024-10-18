#include "Server.h"
#include "Config.h"
#include <EEPROM.h>
#include "HWFunction.h"

AsyncWebServer server(80);
void handleRoot(AsyncWebServerRequest *request)
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

// Function to read servo values from EEPROM
void readServoValues(int &servo1, int &servo2, int &servo3)
{
  // Initialize default values
  servo1 = 0;
  servo2 = 0;
  servo3 = 0;

  // Read from EEPROM and verify values
  int tempValue;

  // Check servo1
  EEPROM.get(0, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  { // Assuming servo range 0-180
    servo1 = tempValue;
  }
  else
  {
    EEPROM.put(0, 0); // Store default value
  }

  // Check servo2
  EEPROM.get(4, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  {
    servo2 = tempValue;
  }
  else
  {
    EEPROM.put(4, 0); // Store default value
  }

  // Check servo3
  EEPROM.get(8, tempValue);
  if (tempValue >= 0 && tempValue <= 180)
  {
    servo3 = tempValue;
  }
  else
  {
    EEPROM.put(8, 0); // Store default value
  }

  // Commit any changes if defaults were written
  EEPROM.commit();
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
void generateServoSettingsJSON(AsyncWebServerRequest *request)
{
  int defaultServo1, defaultServo2, defaultServo3;
  // Read the values from EEPROM
  readServoValues(defaultServo1, defaultServo2, defaultServo3);

  String json = "{";
  json += "\"defaultServo1\":" + String(defaultServo1) + ",";
  json += "\"defaultServo2\":" + String(defaultServo2) + ",";
  json += "\"defaultServo3\":" + String(defaultServo3);
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
  myservo1.write(defaultServo1);
  myservo2.write(defaultServo2);;
  myservo3.write(defaultServo3);;
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
  myservo2.write(0);
  isSelected = true;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}

void setServo(AsyncWebServerRequest *request)
{
  Serial.println("set");

  // Check if all parameters exist
  if (request->hasParam("servo1") && request->hasParam("servo2") && request->hasParam("servo3"))
  {
    // Get the values from URL parameters
    int servo1 = request->getParam("servo1")->value().toInt();
    int servo2 = request->getParam("servo2")->value().toInt();
    int servo3 = request->getParam("servo3")->value().toInt();

    // TODO: Add code here to control your servos
    // For example:
    // myServo1.write(servo1);
    // myServo2.write(servo2);
    // myServo3.write(servo3);

    // Store values in EEPROM
    EEPROM.put(0, servo1); // Store servo1 at address 0
    EEPROM.put(4, servo2); // Store servo2 at address 4
    EEPROM.put(8, servo3); // Store servo3 at address 8
    EEPROM.commit();

    // Redirect back to the main page
    request->redirect("/");
  }
  else
  {
    // If parameters are missing, send an error
    request->send(400, "text/plain", "Missing servo parameters");
  }
}
void Home(AsyncWebServerRequest *request)
{
  isStarted = false;
  isPaused = false;
  myservo1.write(defaultServo1);
  myservo2.write(defaultServo2);;
  myservo3.write(defaultServo3);;
  request->send(302, "text/plain", "Redirecting...");
  request->redirect("/");
}