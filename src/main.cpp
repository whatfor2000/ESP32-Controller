#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "Server.h"
#include "Config.h"
#include "HWFunction.h"
#include "M2.h"

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
  // int servoPositions[] = {defaultServo1, defaultServo1 + 10, defaultServo1 + 20, defaultServo1 + 30, defaultServo1 + 40, defaultServo1 + 50, defaultServo1 + 60};
  // if (isStarted)
  // {
  //   digitalWrite(ledStartPin, HIGH);
  //   digitalWrite(ledStopPin, LOW);
  // }
  // else
  // {
  //   digitalWrite(ledStartPin, LOW);
  //   digitalWrite(ledStopPin, HIGH);
  // }
  // updateElapsedTime();

  // // การทำงานในโหมด manual เมื่อ isSelected เป็น true
  // if (isStarted && !isPaused && isManualMode && isSelected)
  // {
  //   int value = readDIPSwitch();
  //   currentvalue = value;
  //   M2Servo.write(0);
  //   if (value >= 0 && value < 7)
  //   {
  //     myservo1.write(servoPositions[value]); // หมุน servo ตามค่าที่อ่านได้จาก DIP switch
  //     positionCount[value]++;
  //     lastServoTime = millis(); // อัพเดทเวลา
  //     isSelected = false;
  //   }
  // }

  // // การทำงานในโหมด random
  // if (isStarted && !isPaused && !isManualMode)
  // {
  //   if (millis() - lastServoTime >= servoInterval)
  //   {
  //     int randomValue = random(0, 7);
  //     currentvalue = randomValue;
  //     M1Servo.write(servoPositions[randomValue]); // หมุน servo ไปยังตำแหน่งที่สุ่มได้
  //     positionCount[randomValue]++;
  //     lastServoTime = millis(); // อัพเดทเวลา
  //   }
  // }

  // // การทำงานของ Servo Scoop
  // if (isStarted && !isPaused)
  // {
  //   if (millis() - lastScoopTime >= scoopInterval)
  //   {
  //     M3Servo.write(30); // หมุน servo scoop เพื่อดันวัตถุ
  //     delay(500);         // รอให้การ scoop เสร็จสิ้น
  //     myservo3.write(defaultServo3);;  // คืนค่า servo กลับตำแหน่งเริ่มต้น
  //     lastScoopTime = millis();
  //   }
  // }

  // // การตรวจจับวัตถุจาก IR sensor
  // if (digitalRead(irReceiverPin) == LOW)
  // {
  //   M2Servo.write(180);
  //   digitalWrite(ledIRPin, HIGH); // เปิด LED แสดงว่ามีการตรวจจับวัตถุ
  // }
  // else
  // {
  //   M2Servo.write(0);
  //   digitalWrite(ledIRPin, LOW); // ปิด LED เมื่อไม่มีการตรวจจับวัตถุ
  // }
}
