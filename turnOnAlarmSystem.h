/**
 * @file turnOnAlarmSystem.h
 * @author Hákon Hlynsson  (s225765@dtu.dk)
 * @brief source file for the alarm system
 * @version 1.0
 * @date 2025-01-15
 * 
 * @copyright open source
 */

#define turnOnAlarmSystem_H

void initAlarmSystem (int buttonPin, int ledPin);

int handleAlarmSystem(int buttonPin, int ledPin);

#endif 
