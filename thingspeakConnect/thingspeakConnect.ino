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
int testData = 9;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
}

void loop() {
  
  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)

  test();

  client.stop();
  alarm = !alarm;
  delay(postDelay);
}

void setDataInt(unsigned int field, int value){
  int x = 0;

  x =  ThingSpeak.setField(field, value);

  if (x == 200){
    Serial.println("setDataInt: Data set succesfully!");
  }else{
    Serial.print("setDataInt: Error ");
    Serial.println(x);
  }
}


void setDataString(unsigned int field, String str){
  int x = 0;

  x =  ThingSpeak.setField(field, str);

  if (x == 200){
    Serial.println("setDataString: Data set succesfully!");
  }else{
    Serial.print("setDataString: Error ");
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
  setDataInt(2, (int)state);
}

int requestData(unsigned int field){
  int readData = ThingSpeak.readIntField(channelID, field, readAPIKey);

  if(readData == 0){
    Serial.println("requestData: Failed");
    return 0;
  }else{
    Serial.println("requestData: Successful");
    return readData;
  }
}
int testInt = 0;

void test(){
  setDataInt(1, testInt++);
  setDataString(3, "abcdabcdabcdabcd");
  //ThingSpeak.setField(3, "ABCD");
  setAlarm(alarm);
  sendUpdate();

  testData = requestData(1);
  Serial.println(testData);
}