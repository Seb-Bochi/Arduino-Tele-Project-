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

typedef struct Device1_message {
    int Detected_Sound;
    int Detected_Motion;
} Device1_message;

typedef struct Device2_message {
    int RFID_Access;
    int Detected_Light;
    String UID;
} Device2_message;




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
    if (strcmp(macStr, "C4:5B:BE:74:50:EF") == 0){ // Data from RFID and Light sensor 
        // Perform specific action for this sender
        memcpy(&Device2, incomingData, sizeof(Device2));
        Serial.println("Action for Device 2");
        Serial.print("RFID_Access: ");
        Serial.println(Device2.RFID_Access);
        Serial.print("Detected_Light: ");
        Serial.println(Device2.Detected_Light);
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
// Check if the alarm is off and handle the button press to activate it
  if (alarm == false) {
    alarm_off(ledPinRed, ledPinGreen, buzzerPin);
    alarm = check_button(ButtonPin); // Check if the button is pressed to turn on the alarm
  } 
  else {
    // Alarm is ON, process incoming messages or burglary status
    if (New_messege || burglary) {
      delay(10);
      New_messege = false;

      // Check if RFID is present to turn off the alarm
      if (Device2.RFID_Access == 1) {
        burglary = false; // Reset burglary flag
        alarm = false;    // Turn off the alarm
        alarm_off(ledPinRed, ledPinGreen, buzzerPin);
        Device1.Detected_Sound=0;
        Device1.Detected_Motion=0;
        Device2.Detected_Light =0;
        Device2.RFID_Access=0;
        // Optionally send the UID to ThingSpeak
        //setUIDString(3, Device2.UID); // Sending UID to ThingSpeak (Field 3)
      }
      else if ((Device1.Detected_Sound == 1) || (Device1.Detected_Motion == 1) || (Device2.Detected_Light == 1) || burglary) {
                  alarm_sound(ledPinRed, ledPinGreen, buzzerPin); // Trigger the alarm sound and lights
                  burglary = true;  // Mark that a burglary occurred
                  //alarm = true;     // Keep the alarm on
                  } 
    } 
    else {
      alarm_on_sound_but_no_sound(ledPinRed, ledPinGreen, buzzerPin);
    }
  }

  // Periodically update ThingSpeak
  if (count >= postDelay) {
    setAlarm((bool)alarm); // Update alarm state on ThingSpeak (Field 2)
    sendUpdate();          // Push data to ThingSpeak
    count = 0;             // Reset counter
  }
 /* if (alarm == false){
        //Serial.println(alarm);
        alarm_off(ledPinRed,ledPinGreen, buzzerPin);
        alarm=check_button(ButtonPin);
        //check if button is pressed to put alarm on   
      }
  else{
    if (New_messege ||burglary ){ // gets a messenge from the other esp
      New_messege=false; 
      if (Device2.RFID_Access == 1){
         burglary=false;
         alarm=false;
         // send the UID to thingspeak
         //setUIDString(3, Device2.UID);
         alarm_off(ledPinRed,ledPinGreen, buzzerPin);
         
      }
      if(Device1.Detected_Sound == 1 || Device1.Detected_Motion == 1 || Device2.Detected_Light == 1|| burglary==true ){
        alarm_sound(ledPinRed,ledPinGreen, buzzerPin); // makes lights blik and buzzer makes sound (alarm has been trigered) 
        burglary=true;
        alarm=true; 
      }

  }
  else{
      alarm_on_sound_but_no_sound(ledPinRed,ledPinGreen, buzzerPin);
      }
  }


  

  if(count == postDelay){
    setAlarm((bool)alarm);
    sendUpdate();
    count = 0;
  }

  //client.stop();

  delay(1);
  count++;*/
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

void setAlarm(bool state){
  setDataInt(2, (int)state);
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
