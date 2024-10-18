#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

//=================================web===================================
// Wi-Fi Credentials
const char *ssid = "BELLY RICE GRAIN CLASSIFICATION MACHINE";
const char *password = "12345678";

// Create web server instance
AsyncWebServer server(80);

// Variables to track time, positions, and state
unsigned long startTime = 0;
unsigned long pauseTime = 0;
unsigned long elapsedTime = 0;
bool isStarted = false;
bool isPaused = false;
bool isManualMode = true;
bool isSelected = false;

//=================================web===================================
//=================================hardware===================================
bool isFirsttime = true;
const int pinTraffic = 19;
const int pinIn[] = {27, 26, 25}; // ขา DIP switch
const int servoPin = 12;
const int servoPin2 = 13;
const int servoScoop = 19;
const int irReceiverPin = 5;

const int ledStartPin = 16; // LED สีเขียวสำหรับ Start
const int ledStopPin = 17;  // LED สีแดงสำหรับ Stop
const int ledIRPin = 4;     // LED สีเหลืองสำหรับตรวจจับ IR sensor

const int numberOfPositions = 7;

// ประกาศ array สำหรับ servo positions
int servoType[numberOfPositions] = {0, 1, 2, 3, 4, 5, 6}; // ใช้ 404 เป็นตัวอย่างของค่าที่จะทำให้แสดง "error"
// ตัวนับตำแหน่ง
int positionCount[7] = {0};
int currentvalue = 0;

// ตัวแปรเวลาสำหรับการควบคุมด้วย millis()
unsigned long lastScoopTime = 0;
unsigned long scoopInterval = 1000; // ระยะเวลาที่ใช้สำหรับการ scoop
unsigned long lastServoTime = 0;
unsigned long servoInterval = 2000; // ระยะเวลาสำหรับ servo

// สร้าง object servo
Servo myservo1;
Servo myservo2;
Servo myservo3;

int defaultServo1 = 0;
int defaultServo2 = 180;
int defaultServo3 = 0;
//=================================hardware===================================

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

// ฟังก์ชันสำหรับจัดรูปแบบเวลาเป็นชั่วโมง:นาที:วินาที
String formatTime(unsigned long milliseconds)
{
  unsigned long totalSeconds = milliseconds / 1000;
  int hours = totalSeconds / 3600;
  int minutes = (totalSeconds % 3600) / 60;
  int seconds = totalSeconds % 60;

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
  return String(buffer);
}

int calculateTotalCount()
{
  int total = 0;
  for (int i = 0; i < 7; i++)
  {
    total += positionCount[i];
  }
  return total;
}

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
void start()
{
  if (isStarted)
  {
    if (isPaused) // If currently paused, we want to resume
    {
      isPaused = false;
      startTime += millis() - pauseTime; // Adjust start time to continue counting from where it paused
    }
    else // If running, we want to pause
    {
      isPaused = true;
      pauseTime = millis(); // Record the time when paused
    }
  }
  else // Starting for the first time
  {
    isStarted = true;
    isPaused = false;

    if (isFirsttime)
    {
      startTime = millis(); // Start fresh
      lastScoopTime = millis();
      lastServoTime = millis();
      isFirsttime = false; // Set the first time flag to false after starting
    }
    else
    {
      startTime = millis() - elapsedTime; // Continue from where it was paused
    }
  }
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

void updateElapsedTime()
{
  if (isStarted && !isPaused)
  {
    elapsedTime = millis() - startTime;
  }
}

int readDIPSwitch()
{
  int value = 0;
  for (int i = 0; i < 3; i++)
  {
    value |= (digitalRead(pinIn[i]) == LOW) << i;
  }
  return value;
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

void setup()
{

  // Initialize EEPROM with size enough to store your data
  EEPROM.begin(12); // 4 bytes per int, total of 3 integers

  pinMode(irReceiverPin, INPUT_PULLUP);
  pinMode(ledIRPin, OUTPUT);
  for (int i = 0; i < 3; i++)
  {
    pinMode(pinIn[i], INPUT_PULLUP);
  }
  pinMode(ledStartPin, OUTPUT);
  pinMode(ledStopPin, OUTPUT);

  myservo1.attach(servoPin);
  myservo2.attach(servoPin2);
  myservo3.attach(servoScoop);

  // Home();
  myservo1.write(defaultServo1);
  myservo2.write(defaultServo2);;
  myservo3.write(defaultServo3);;

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
  server.on("/", handleRoot);
  server.on("/data", HTTP_GET, generateJSON);
  server.on("/getservo", HTTP_GET, generateServoSettingsJSON);
  server.on("/pause", HTTP_GET, Pause);
  server.on("/reset", HTTP_GET, reset);
  server.on("/home", HTTP_GET, Home);

  server.on("/start/manual", HTTP_GET, manualmode);
  server.on("/start/random", HTTP_GET, randommode);
  server.on("/selected", HTTP_GET, selected);
  server.on("/setservo", HTTP_GET, setServo);

  // Start the server
  server.begin();
}
void loop()
{

  int servoPositions[] = {defaultServo1, defaultServo1 + 10, defaultServo1 + 20, defaultServo1 + 30, defaultServo1 + 40, defaultServo1 + 50, defaultServo1 + 60};
  if (isStarted)
  {
    digitalWrite(ledStartPin, HIGH);
    digitalWrite(ledStopPin, LOW);
  }
  else
  {
    digitalWrite(ledStartPin, LOW);
    digitalWrite(ledStopPin, HIGH);
  }
  updateElapsedTime();

  // การทำงานในโหมด manual เมื่อ isSelected เป็น true
  if (isStarted && !isPaused && isManualMode && isSelected)
  {
    int value = readDIPSwitch();
    currentvalue = value;
    myservo2.write(0);
    if (value >= 0 && value < 7)
    {
      myservo1.write(servoPositions[value]); // หมุน servo ตามค่าที่อ่านได้จาก DIP switch
      positionCount[value]++;
      lastServoTime = millis(); // อัพเดทเวลา
      isSelected = false;
    }
  }

  // การทำงานในโหมด random
  if (isStarted && !isPaused && !isManualMode)
  {
    if (millis() - lastServoTime >= servoInterval)
    {
      int randomValue = random(0, 7);
      currentvalue = randomValue;
      myservo1.write(servoPositions[randomValue]); // หมุน servo ไปยังตำแหน่งที่สุ่มได้
      positionCount[randomValue]++;
      lastServoTime = millis(); // อัพเดทเวลา
    }
  }

  // การทำงานของ Servo Scoop
  if (isStarted && !isPaused)
  {
    if (millis() - lastScoopTime >= scoopInterval)
    {
      myservo3.write(30); // หมุน servo scoop เพื่อดันวัตถุ
      delay(500);         // รอให้การ scoop เสร็จสิ้น
      myservo3.write(defaultServo3);;  // คืนค่า servo กลับตำแหน่งเริ่มต้น
      lastScoopTime = millis();
    }
  }

  // การตรวจจับวัตถุจาก IR sensor
  if (digitalRead(irReceiverPin) == LOW)
  {
    myservo2.write(180);
    digitalWrite(ledIRPin, HIGH); // เปิด LED แสดงว่ามีการตรวจจับวัตถุ
  }
  else
  {
    myservo2.write(0);
    digitalWrite(ledIRPin, LOW); // ปิด LED เมื่อไม่มีการตรวจจับวัตถุ
  }
}
