#ifndef senderESP_H
#define senderESP_H

#include <ESP8266WiFi.h>
#include <espnow.h>

void onDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void initESPNow();
void registerPeer(uint8_t* macAddress);
void sendData(uint8_t* macAddress, void* data, size_t dataSize);


#endif 
