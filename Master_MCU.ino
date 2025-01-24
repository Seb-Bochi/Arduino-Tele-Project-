/**
 * @file Master_MCU.ino
 * @author Hákon Hlynsson (s225765@dtu.dk)
 * @brief this is the main file for the master esp
 * @version 1.0
 * @date 2025-01-19
 * 
 * @copyright open source
 * 
 */
#include <espnow.h>

const int ledPinRed = D3;
const int ledPinGreen = D4;
const int buzzerPin = D8;
const int ButtonPin=D7;
bool New_messege = false;
bool burglary = false;
int alarm=false;
int prev_buttonState = false;

/**
 * @brief struct to hold the data from device 2
 *
 * 
 */
typedef struct Device2_message {
    int Detected_Light;
    int RFID_Access;
    float UID;
} Device2_message;

/**
 * @brief struct to hold the data from device 1
 *
 * 
 */
typedef struct Device1_message {
    int Detected_Sound;
    int Detected_Motion;
} Device1_message;


// Create a struct_message called myData
Device1_message Device1;
Device2_message Device2;

// Callback function that will be executed when data is received

/**
 * @brief function to handle the data received from the other esp
 * 
 * @param mac mac address of the sender
 * @param incomingData incoming data from the sender
 * @param len length of the incoming data 
 */
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
}

void loop() {

if (alarm == false){
      Serial.println(alarm);
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
  
}

/**
 * @brief initialize the alarm system
 * 
 * @param Red_LedPin the pin for the red led
 * @param Green_LedPin the pin for the green led
 * @param BuzzerPin the pin for the buzzer
 * @param Button_Pin the pin for the button
 */
void initAlarm(int Red_LedPin,int Green_LedPin, int BuzzerPin,int Button_Pin){
  pinMode(Button_Pin, INPUT_PULLUP);
  pinMode(Red_LedPin, OUTPUT);
  pinMode(Green_LedPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
}

/**
 * @brief function to make the alarm sound
 * 
 * @param Red_LedPin the pin for the red led
 * @param Green_LedPin the pin for the green led
 * @param BuzzerPin the pin for the buzzer
 */
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
/**
 * @brief alarm on but no sound
 * 
 * @param Red_LedPin the pin for the red led
 * @param Green_LedPin the pin for the green led
 * @param BuzzerPin the pin for the buzzer
 */
void alarm_on_sound_but_no_sound(int Red_LedPin,int Green_LedPin, int BuzzerPin){
      digitalWrite(Red_LedPin, HIGH);
      digitalWrite(Green_LedPin, LOW);
      digitalWrite(buzzerPin, LOW);
}

/**
 * @brief alarm off
 * 
 * @param Red_LedPin  red led pin
 * @param Green_LedPin green led pin
 * @param BuzzerPin buzzer pin
 */
void alarm_off(int Red_LedPin,int Green_LedPin, int BuzzerPin){
  digitalWrite(Red_LedPin, LOW);
  digitalWrite(Green_LedPin, HIGH);
  digitalWrite(buzzerPin, LOW);
}

/**
 * @brief function to check if the button is pressed
 * 
 * @param Button_Pin the pin for the button
 * @return int true if the button is pressed
 */
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
