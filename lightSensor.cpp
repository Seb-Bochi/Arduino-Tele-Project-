#include "lightSensor.h"
#include <Arduino.h>

int read_Photoresistor(int pin ){
  int value=analogRead(pin);
  return value;
}

int initialize_Photoresistor(int pin) {
  pinMode(pin, INPUT);
  int calibrationValue = calibrate_Photoresistor(pin);
  return calibrationValue;
}

int calibrate_Photoresistor(int pin) {
  int total = 0;
  // Read the photoresistor 100 times and average the values
  for (int i = 0; i < 100; i++) {
    total += analogRead(pin);
    if (i>=1){
      total/=2;
    }
  }
  return total;
}

int check_for_change_Photoresistor(int Photoresistor_value,int calibration,int threeshold){
  int alarm;
  if (Photoresistor_value > ( calibration+threeshold)||Photoresistor_value < ( calibration-threeshold)){
    alarm=1;
  }
  else{
    alarm=0;
  }

  return alarm;
}
