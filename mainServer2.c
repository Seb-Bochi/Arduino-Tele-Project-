/* This is the  main code for node 2. Its functions are: 
 * 1. To monitor input from RFID and a photoresister.
 * 2. To use the ESP8288 to transmit the sensor data wireless to another ESP8288 using ESP-NOW. 
 * 
 * Features:
 * RFID reader, that reads the UID of a tag. Tags with allowed access transmit a "flag" that can be used by another ESP8288. 
 * The UID is also trasmitted. The allowed UIDs is predefined in the code. 
 * The photo resister transmit a flag if activated. 
 *
 * Debugging: 
 * The code outputs sensor status, RFID alarm status and the UID to the serial monitor. 
 *
 * Note: 
 * The MAC address for the resieving ESP is stored in this code. 

*/





#include "RFID.h"
#include <Arduino.h>
#include "lightSensor.h"
#include "senderESP.h"

#define SS_PIN D8
#define RST_PIN D2

struct CustomData {
    int RFIDAlarm;
    int lightSensorAlarm;
    String UID; 
};



// MAC address variable
uint8_t broadcastAddress[6] = {0xdc, 0x4f, 0x22, 0x18, 0xea, 0x8a};


String knownUIDs[] = { "4D028A3F", "9314F4E1" };

int photopin = A0;
int calibrated_Photoresistor = 0;
int value_Photoresistor=0;
int threeshold_Photoresistor=100;
int alarm_sound_2 = 0; 
const char* ssid = "Mikkels phone";
const char* pass = "456789er"; 

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(9600);
    RFIDsetup();  // Initialize RFID module
    calibrated_Photoresistor = initialize_Photoresistor(photopin);
    initESPNow();
    registerPeer(broadcastAddress);
    WiFi.begin(ssid, pass);
}

void loop() {
  int value_Photoresistor = read_Photoresistor(photopin);
  int alarmPhoto = check_for_change_Photoresistor(value_Photoresistor, calibrated_Photoresistor, threeshold_Photoresistor);
    RFIDResult result = readUID();  // Read UID and get the result



  CustomData dataToSend;
      //dataToSend.RFIDAlarm = result.approved;
      dataToSend.RFIDAlarm = result.approved;
      dataToSend.lightSensorAlarm = alarmPhoto;
      dataToSend.UID = result.content;
     
      

      Serial.print("Light sensor: ");
      Serial.println(alarmPhoto);
      Serial.print("RFID: ");
      Serial.println(result.approved);
      Serial.print("UID: ");
      Serial.println(result.content);

     if(result.approved == 1|| alarmPhoto == 1){
    sendData(broadcastAddress, &dataToSend, sizeof(dataToSend));
    
      
    }

}