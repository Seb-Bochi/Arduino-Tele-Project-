#ifndef soundSensor_H
#define soundSensor_H

int initialize_sound(int pin);

int Calibration_of_sound();

int check_for_change_in_sound(int Sound_Value, int Calibration, int thressshold);

int read_sound_value();

#endif 
