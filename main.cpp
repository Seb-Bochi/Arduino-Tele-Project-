/**
 * @file main.cpp
 * @author Hákon Hlynsson (s225765@dtu.dk)
 * @brief main file for the alarm system
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 */
#include "turnOnAlarmSystem.h"
#include "alarmIsActivated.h"
#include "ultrasonicSensor.h"


const int buttonPin = 8;
const int ledPinGreen = 7;
const int ledPinRed = 6;
const int buzzerPin = 5;
const int trigPin = 9;
const int echoPin = 10;

int alarmIsOn = 0;

/**
 * @brief setup function for the alarm system
 * 
 */
void setup (){
  initAlarmSystem (buttonPin, ledPinGreen);
  initAlarmIsActivated(buttonPin, ledPinRed, buzzerPin);
  initUltrasonicSensor(trigPin, echoPin);

  Serial.begin(9600);
}
/**
 * @brief loop function for the alarm system
 * 
 */
void loop(){

  int ledState = handleAlarmSystem(buttonPin, ledPinGreen);
   if (ledState == 1){
    Serial.println("Alarm is ready");
   } else {
      Serial.println("Alarm is NOT ready");
   }

  if (ledState == 1 && ultrasonicSensor(trigPin, echoPin) == 1){
    alarmIsActivated(buttonPin, ledPinRed, buzzerPin);
    Serial.println("Alarm is ON");
  } else {
    alarmIsNOTActivated(buttonPin, ledPinRed, buzzerPin);
    Serial.println("Alarm is OFF");
  }

  delay(100);

}