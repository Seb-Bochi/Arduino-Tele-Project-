/**
 * @file turnonAlarmSystem.cpp
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief  source file for the alarm system
 * @version 1.0
 * @date 2025-01-15
 * 
 * @copyright open source
 * 
 */
#include "turnOnAlarmSystem.h"
#include <Arduino.h>

int buttonState = 0;
int lastButtonState = 0;
int ledState = 0;

/**
 * @brief initialize the the pins for the alarm system
 * 
 * @param buttonPin the pin for the button
 * @param ledPin the pin for the led
 */
void initAlarmSystem (int buttonPin, int ledPin) {


  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}


/**
 * @brief handle the alarm system
 * 
 * @param buttonPin the pin for the button
 * @param ledPin the pin for the led
 * @return int the state of the led
 */
int handleAlarmSystem(int buttonPin, int ledPin) {
  // read the state of the pushbutton value:

  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW && lastButtonState == HIGH) {
   delay(50);
    if(buttonState == LOW){
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }

  lastButtonState = buttonState;

 return ledState;

}
