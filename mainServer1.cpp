#include "ultrasonicSensor.h"
#include "soundSensor.h"
#include <Arduino.h>
#include "senderESP.h"

struct CustomData {
    int soundSensorAlarm;
    int ultrasonicSensorAlarm;
};

// MAC address variable
uint8_t broadcastAddress[6] = {0xdc, 0x4f, 0x22, 0x18, 0xea, 0x8a};

const int trigPin = D2;
const int echoPin = D3;

const int SensorPin = A0;            //< Analog pin to which the KY-037 sensor is connected
int Calibration_Value = 0;     //< Baseline calibration value for sound sensor
const int threeshold_value = 40;     //< Threshold for detecting significant sound changes



void setup (){
  initUltrasonicSensor(trigPin, echoPin);

  Calibration_Value = initialize_sound(SensorPin); 

  initESPNow();
  registerPeer(broadcastAddress);

  Serial.begin(9600);
}



void loop(){  
  int alarm_sound = check_for_change_in_sound(read_sound_value(), Calibration_Value, threeshold_value);
  int alarm_sound_2 = ultrasonicSensor(trigPin, echoPin);

    CustomData dataToSend;
    dataToSend.soundSensorAlarm = alarm_sound;
    dataToSend.ultrasonicSensorAlarm = alarm_sound_2;

    Serial.print("sound sensor: ");
    Serial.println(alarm_sound);
    Serial.print("Ultrasound Sensor: ");
    Serial.println(alarm_sound_2);

    sendData(broadcastAddress, &dataToSend, sizeof(dataToSend));


 
  delay(100);
}