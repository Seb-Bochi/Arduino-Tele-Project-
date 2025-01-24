/**
 * @file soundSensor.c
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief source file for the sound sensor
 * @version 1.0
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "soundSensor.h"
#include <Arduino.h>

/**
 * @brief Arduino setup function. Initializes the serial communication and calibrates the sound sensor.
 */

/**
 * @brief Initializes the sound sensor and performs calibration.
 *
 * @param pin The analog pin to which the sensor is connected.
 * @return The calibration value for the sound sensor.
 */
int initialize_sound(int pin) {
  int Calibration = 0;
  pinMode(pin, INPUT); ///< Set the sensor pin as an input
  Calibration = Calibration_of_sound(); ///< Perform sound calibration
  return Calibration;
}

/**
 * @brief Calibrates the sound sensor by averaging a 1000 sound readings.
 *
 * @return The calculated calibration value.
 */
int Calibration_of_sound() {
  int sound_value = 0;
  for (int i = 0; i < 1000; i++) {
    sound_value += read_sound_value(); ///< Accumulate sound readings
    if (i >= 1) {
      sound_value /= 2; ///< Compute running average
    }
  }
  return sound_value;
}

/**
 * @brief Checks if the current sound value deviates from the calibration value by more than the threshold.
 *
 * @param Sound_Value The current sound level reading.
 * @param Calibration The baseline calibration value.
 * @param thressshold The threshold for detecting significant sound changes.
 * @return True if the sound level change exceeds the threshold; otherwise, false.
 */
int check_for_change_in_sound(int Sound_Value, int Calibration, int thressshold) {
  int alarm;
  if (Sound_Value > (Calibration + thressshold) || Sound_Value < (Calibration - thressshold)) {
    alarm = 1; ///< Set alarm if sound level deviates significantly
  } else {
    alarm = 0; ///< No significant deviation detected
  }
  return alarm;
}

/**
 * @brief Reads the sound level from the sensor by averaging multiple analog readings.
 *
 * @return The averaged sound level.
 */
int read_sound_value() {
  int SensorVal = 0; ///< Variable to store analog readings
  for (int i = 0; i < 10; i++) {
    SensorVal = analogRead(SensorPin); ///< Read analog value from sensor
    if (i >= 1) {
      SensorVal /= 2; ///< Compute running average
    }
  }
  return SensorVal;
}
