#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ThingSpeak.h>

const char* ssid = "Mikkels phone";
const char* pass = "456789er";
WiFiClient client;
unsigned long channelID = 2809451; //Channel ID of the ThingSpeak channel
const char * writeAPIKey = "0LYBHH8XR80QLZIN"; //Write API key for the ThingSpeak Channel
const char* readAPIKey = "3BVUMEF3172HF9DV"; //Read  API key for the ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postDelay = 20 * 1000;
int count = 0;

const int ledPinRed = D3;
const int ledPinGreen = D4;
const int buzzerPin = D8;
const int ButtonPin=D7;
bool New_messege = false;
bool burglary = false;
int alarm=false;
int prev_buttonState = false;


typedef struct Device2_message {
    int Detected_Light;
    int RFID_Access;
    String UID;
} Device2_message;

typedef struct Device1_message {
    int Detected_Sound;
    int Detected_Motion;
} Device1_message;


// Create a struct_message called myData
Device1_message Device1;
Device2_message Device2;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  // Convert MAC address to human-readable string
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", 
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  
   // Example: Act based on sender's MAC address
    if (strcmp(macStr, "C8:C9:A3:69:D3:F9") == 0) {
         memcpy(&Device1, incomingData, sizeof(Device1));
        Serial.println("Action for Device 1");
        Serial.print("Detected_Sound: ");
        Serial.println(Device1.Detected_Sound);
        Serial.print("Detected_Motion: ");
        Serial.println(Device1.Detected_Motion);
    }   
    if (strcmp(macStr, "YY:YY:YY:YY:YY:YY") == 0){ // change it to the mac address for the EPS sender 
        // Perform specific action for this sender
        Serial.println("Action for Device 2");
        Serial.print("Detected_Light: ");
        Serial.println(Device2.Detected_Light);
        Serial.print("Detected_Light: ");
        Serial.println(Device2.RFID_Access);
        Serial.print("UID: ");
        Serial.println(Device2.UID); 
    }   
    else{
        Serial.println("Unknown Device");
    }

  New_messege=true;
}

 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  initAlarm(ledPinRed,ledPinGreen, buzzerPin,ButtonPin);
  
  // Connect device to Wifi
  WiFi.begin(ssid, pass);

  // Set device as a Wi-Fi Station to cormect to the other 
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE); // sets the device as a revicer of data 
  esp_now_register_recv_cb(OnDataRecv); 

  ThingSpeak.begin(client);
  client.connect(server, 80); //connect(URL, Port)
}

void loop() {

  if (alarm == false){
        //Serial.println(alarm);
        alarm_off(ledPinRed,ledPinGreen, buzzerPin);
        alarm=check_button(ButtonPin);
        //check if button is pressed to put alarm on   
      }
  else{
    alarm_on_sound_but_no_sound(ledPinRed,ledPinGreen, buzzerPin);
  }


  if (New_messege){ // gets a messenge from the other esp
    if (alarm==true){  
      if (Device2.RFID_Access == 1){
         burglary=false;
         alarm=false;
         // send the UID to thingspeak
         setUIDString(3, Device2.UID);

         alarm_off(ledPinRed,ledPinGreen, buzzerPin);
         
       }
      if(Device1.Detected_Sound == 1 || Device1.Detected_Motion == 1 || Device2.Detected_Light == 1|| burglary==true ){
        alarm_sound(ledPinRed,ledPinGreen, buzzerPin); // makes lights blik and buzzer makes sound (alarm has been trigered) 
        burglary=true; 
        }
       
       else{
         alarm_on_sound_but_no_sound(ledPinRed,ledPinGreen, buzzerPin);
       }
    }   
  New_messege=false; 
  }

  if(count == postDelay){
    sendUpdate();
    count = 0;
  }

  //client.stop();

  delay(1);
  count++;
}


void initAlarm(int Red_LedPin,int Green_LedPin, int BuzzerPin,int Button_Pin){
  pinMode(Button_Pin, INPUT_PULLUP);
  pinMode(Red_LedPin, OUTPUT);
  pinMode(Green_LedPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
}

void alarm_sound(int Red_LedPin,int Green_LedPin, int BuzzerPin){
      digitalWrite(Red_LedPin, HIGH);
      digitalWrite(Green_LedPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(10);
      digitalWrite(Red_LedPin, LOW);
      digitalWrite(Green_LedPin, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(10);
}

void alarm_on_sound_but_no_sound(int Red_LedPin,int Green_LedPin, int BuzzerPin){
      digitalWrite(Red_LedPin, HIGH);
      digitalWrite(Green_LedPin, LOW);
      digitalWrite(buzzerPin, LOW);
}

void alarm_off(int Red_LedPin,int Green_LedPin, int BuzzerPin){
  digitalWrite(Red_LedPin, LOW);
  digitalWrite(Green_LedPin, HIGH);
  digitalWrite(buzzerPin, LOW);
}

int check_button(int Button_Pin){
  // Read the current state of the pushbutton
   int buttonState = digitalRead(Button_Pin);
   // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW && prev_buttonState == HIGH) {
    delay(50);
    buttonState = digitalRead(Button_Pin);
    if(buttonState == LOW){
      Serial.println("Button_pressed");
      prev_buttonState = buttonState;
      return true;
    }
  }
  prev_buttonState = buttonState; 
  return false;
}

void setUIDString(unsigned int field, String str){
  int x = 0;

  x =  ThingSpeak.setField(field, str);

  if (x == 200){
    Serial.println("setUIDString: Data set succesfully!");
  }else{
    Serial.print("setUIDString: Error ");
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
