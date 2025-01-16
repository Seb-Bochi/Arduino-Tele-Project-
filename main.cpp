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

void setup (){
  initAlarmSystem (buttonPin, ledPinGreen);
  initAlarmIsActivated(buttonPin, ledPinRed, buzzerPin);
  initUltrasonicSensor(trigPin, echoPin);

  Serial.begin(9600);
}

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