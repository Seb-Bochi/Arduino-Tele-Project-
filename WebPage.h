#ifdef webPage_H
#define webPage_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>


void websiteBegin(const char** ssid,const char** password );

void websiteLoop(bool* armed, bool* breakein, String tag);

#endif