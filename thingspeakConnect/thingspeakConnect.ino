#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
const char* ssid = "Mikkels phone";
const char* pass = "456789er";
WiFiClient client;
unsigned long channelID = 2809451; //Channel ID of the ThingSpeak channel
const char * writeAPIKey = "0LYBHH8XR80QLZIN"; //Write API key for the ThingSpeak Channel
const char* readAPIKey = "3BVUMEF3172HF9DV"; //Read  API key for the ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postDelay = 20 * 1000;

bool alarm = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
}

void loop() {
  
  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)

  setData(1, 5);
  setAlarm(alarm);
  sendUpdate();
  alarm = !alarm;
  delay(postDelay);
}

void setData(unsigned int field, int value){
  int x = 0;

  x =  ThingSpeak.setField(field, value);

  if (x == 200){
    Serial.println("setData: Data set succesfully!");
  }else{
    Serial.print("setData: Error ");
    Serial.println(x);
  }
}

void sendUpdate(){
  int x = 0;
  x = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (x == 200){
    Serial.println("sendUpdate: Update sent succesfully!");
  }else{
    Serial.print("sendUpdate: Error");
    Serial.println(x);
  }
}

void setAlarm(bool state){
  setData(2, (int)state);
}

int requestData(unsigned int field){
  
  return 0;
}