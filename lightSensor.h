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
#ifndef lightSensor_H
#define lightSensor_H

int read_Photoresistor(int pin);

int initialize_Photoresistor(int pin);

int calibrate_Photoresistor(int pin);

int check_for_change_Photoresistor(int Photoresistor_value,int calibration,int threeshold);

#endif 
