/**
 * @file alarmIsActivated.cpp
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief file for activating the alarm
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 */

#include "alarmIsActivated.h"
#include <Arduino.h>


// variable for reading the pushbutton status
/**
 * @brief function to initialize the alarm system
 * 
 * @param buttonPin the pin for the button
 * @param ledPin the pin for the led
 * @param buzzerPin the pin for the buzzer
 */
void initAlarmIsActivated(int buttonPin, int ledPin, int buzzerPin) {


  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(buzzerPin, OUTPUT);  
}
/**
 * @brief function to handle the alarm system
 * 
 * @param buttonPin the pin for the button
 * @param ledPin the pin for the led
 * @param buzzerPin the pin for the buzzer
 * @return int the state of the led
 */
int alarmIsActivated(int buttonPin, int ledPin, int buzzerPin) { 
  // read the state of the pushbutton value:
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(150);
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(150);

      return 1;
}
/**
 * @brief function to turn off the alarm
 * 
 * @param buttonPin  the pin for the button
 * @param ledPin the pin for the led
 * @param buzzerPin the pin for the buzzer
 * @return int the state of the alarm
 */
int alarmIsNOTActivated(int buttonPin, int ledPin, int buzzerPin) { 
  // read the state of the pushbutton value:
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

      
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      

      return 0;
}
