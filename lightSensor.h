#ifndef lightSensor_H
#define lightSensor_H

int read_Photoresistor(int pin);

int initialize_Photoresistor(int pin);

int calibrate_Photoresistor(int pin);

int check_for_change_Photoresistor(int Photoresistor_value,int calibration,int threeshold);

#endif 
