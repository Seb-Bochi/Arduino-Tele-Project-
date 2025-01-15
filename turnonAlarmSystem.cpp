#include "turnOnAlarmSystem.h"
#include <Arduino.h>

int buttonState = 0;
int lastButtonState = 0;
int ledState = 0;


void initAlarmSystem (int buttonPin, int ledPin) {


  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

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
