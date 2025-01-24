/**
 * @file lightSensor.cpp
 * @author Hákon Hlynsson (s225765@dtu.dk)
 * @brief file for the light sensor
 * @version 1.0
 * @date 2025-01-17
 * 
 * @copyright open source
 * 
 */
#include "lightSensor.h"
#include <Arduino.h>
/**
 * @brief function to read the value of the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int the value of the photoresistor
 */
int read_Photoresistor(int pin ){
  int value=analogRead(pin);
  return value;
}

/**
 * @brief function to initialize the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int the calibration value
 */
int initialize_Photoresistor(int pin) {
  pinMode(pin, INPUT);
  int calibrationValue = calibrate_Photoresistor(pin);
  return calibrationValue;
}

/**
 * @brief function to calibrate the photoresistor
 * 
 * @param pin the pin to which the photoresistor is connected
 * @return int returns the calibration value
 */
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
/**
 * @brief check if the photoresistor value has changed significantly
 * 
 * @param Photoresistor_value the current value of the photoresistor
 * @param calibration the calibration value for the photoresistor
 * @param threeshold the threshold for detecting significant light changes
 * @return int true or false depending on if the light level change exceeds the threshold
 */
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
