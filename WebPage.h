/**
 * @file WebPage.h
 * @author Magnus Ludsgaard(s234393@dtu.dk)
 * @brief source file for the web page
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 */
#ifdef webPage_H
#define webPage_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>


void websiteBegin(const char** ssid,const char** password );

void websiteLoop(bool* armed, bool* breakein, String tag);

#endif