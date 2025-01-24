/**
 * @file alarmIsActivated.cpp
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief file for activating the alarm
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 */
#ifndef alarmIsActivated_H
#define alarmIsActivated_H

void initAlarmIsActivated(int buttonPin, int ledPin, int buzzerPin);

int alarmIsActivated(int buttonPin, int ledPin, int buzzerPin);

int alarmIsNOTActivated(int buttonPin, int ledPin, int buzzerPin);

#endif 
