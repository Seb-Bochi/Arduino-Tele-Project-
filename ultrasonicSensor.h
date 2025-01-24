/**
 * @file UltrasonicSensor
 * @author Sebastian Bochineck (s224014@dtu.dk)
 * @brief source file for the ultrasonic sensor
 * @version 1.0
 * @date 2025-01-17
 * 
 * @copyright open source
 */
#ifndef ultrasonicSensor_H
#define ultrasonicSensor_H

void initUltrasonicSensor(int trigPin, int echoPin);

int ultrasonicSensor(int trigPin, int echoPin);

#endif 
