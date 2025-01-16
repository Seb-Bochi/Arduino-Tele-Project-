#include "alarmIsActivated.h"
#include <Arduino.h>


// variable for reading the pushbutton status

void initAlarmIsActivated(int buttonPin, int ledPin, int buzzerPin) {


  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  pinMode(buzzerPin, OUTPUT);  
}

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

int alarmIsNOTActivated(int buttonPin, int ledPin, int buzzerPin) { 
  // read the state of the pushbutton value:
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:

      
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      

      return 0;
}
