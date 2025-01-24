/**
 * @file soundSensor.h
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief source file for the sound sensor
 * @version 1.0
 * @date 2025-01-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef soundSensor_H
#define soundSensor_H

int initialize_sound(int pin);

int Calibration_of_sound();

int check_for_change_in_sound(int Sound_Value, int Calibration, int thressshold);

int read_sound_value();

#endif 
