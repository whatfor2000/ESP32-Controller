#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "Server.h"
#include "Config.h"
#include "HWFunction.h"

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
