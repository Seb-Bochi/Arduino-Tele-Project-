#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ThingSpeak.h>
#include <NTPClient.h>


const char* ssid = "Mikkels phone";
const char* pass = "456789er";
WiFiClient client2;
unsigned long channelID = 2809451; //Channel ID of the ThingSpeak channel
const char * writeAPIKey = "0LYBHH8XR80QLZIN"; //Write API key for the ThingSpeak Channel
const char* readAPIKey = "3BVUMEF3172HF9DV"; //Read  API key for the ThingSpeak Channel
const char* server = "api.thingspeak.com";
unsigned long lastUpdate = 0;
const unsigned long postDelay = 20*1000;
int count = 0;

WiFiClient client;

const int ledPinRed = D3;
const int ledPinGreen = D4;
const int buzzerPin = D8;
const int ButtonPin=D7;
bool New_messege = false;
bool burglary = false;
bool alarm=false;
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

WiFiServer espServer(80); /* Instance of WiFiServer with port number 80 */
/* 80 is the Port Number for HTTP Web Server */

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
  websiteBegin(&ssid,&pass);
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

  ThingSpeak.begin(client2);
  client2.connect(server, 80); //connect(URL, Port)
}
void loop() {
  Websiteloop(&alarm,&burglary,Device2.UID);

// Check if the alarm is off and handle the button press to activate it
  if (alarm == false) {
    alarm_off(ledPinRed, ledPinGreen, buzzerPin);
    alarm = check_button(ButtonPin); // Check if the button is pressed to turn on the alarm
  } 
  else {
    // Alarm is ON, process incoming messages or burglary status
    if (New_messege || burglary) {
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
        setUIDString(3, Device2.UID); // Sending UID to ThingSpeak (Field 3)
      }
      else if ((Device1.Detected_Sound == 1) || (Device1.Detected_Motion == 1) || (Device2.Detected_Light == 1) || (burglary)) {
                  alarm_sound(ledPinRed, ledPinGreen, buzzerPin); // Trigger the alarm sound and lights
                  burglary = true;  // Mark that a burglary occurred
                  } 
    } 
    else {
      alarm_on_sound_but_no_sound(ledPinRed, ledPinGreen, buzzerPin);
    }
  }

  // Periodically update ThingSpeak
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= postDelay) {
    lastUpdate = currentMillis;
    setAlarm((bool)burglary); // Update alarm state on ThingSpeak (Field 2)
    sendUpdate();          // Push data to ThingSpeak
    //count = 0;             // Reset counter
  }
 if ((currentMillis - lastUpdate)%1000==1){
    Serial.println((currentMillis - lastUpdate));
 }

  //delay(1);
  //count++;
}


void websiteBegin(const char** ssid,const char** password ) {
  Serial.print("\n");
  Serial.print("Connecting to: ");
  Serial.println(*ssid);
  WiFi.mode(WIFI_STA);        /* Configure ESP8266 in STA Mode */
  WiFi.begin(*ssid, *password); /* Connect to Wi-Fi based on above SSID and Password */
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("*");
    delay(500);
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  delay(100);
  Serial.print("\n");
  Serial.println("Starting ESP8266 Web Server...");
  espServer.begin(); /* Start the HTTP web Server */
  Serial.println("ESP8266 Web Server Started");
  Serial.print("\n");
  Serial.print("The URL of ESP8266 Web Server is: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
  Serial.println("Use the above URL in your Browser to access ESP8266 Web Server\n");
}


/**
 * @brief A function to create a web page for the security system
 * 
 * @param armed check if the alarm is armed
 * @param breakein check if there is a break-in
 * @param tag display the last tag registered
 */

void Websiteloop(bool* armed, bool* breakein, String tag) {
  client = espServer.available(); /* Check if a client is available */
  if (!client) {
    return;
  }

  Serial.println("New Client!!!");

  String request = client.readStringUntil('\r'); /* Read the first line of the request from client */
  Serial.println(request);                       /* Print the request on the Serial monitor */
  /* The request is in the form of HTTP GET Method */
  client.flush();

  /* Extract the URL of the request */
  if (request.indexOf("/armedON") != -1) {
    Serial.println("The alarm is armed");
    *armed = true;
  }
  if (request.indexOf("/armedOFF") != -1) {
    Serial.println("The alarm is disarmed");
    *armed = false;
  }
  if (request.indexOf("/breakeinON") != -1) {
    Serial.println("There is a break-in");
    //*armed = true;
    *breakein = true;
  }
  if (request.indexOf("/breakeinOFF") != -1) {
    Serial.println("No break-in detected");
    *breakein = false;
  }

  /* HTTP Response in the form of HTML Web Page */
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();  //  IMPORTANT
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  /* CSS Styling for Buttons and Web Page */
  client.println("<style>");
  client.println("html { font-family: Courier New; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button {border: none; color: white; padding: 10px 20px; text-align: center;");
  client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
  client.println(".button1 {background-color: #00FF00;}");
  client.println(".button2 {background-color: #FF0000;}");
  client.println("</style>");
  client.println("</head>");

  /* The main body of the Web Page */
  client.println("<body>");
  client.println("<h2>Security system</h2>");

  if (*armed == false) {
    client.println("<p>Security system is disarmed</p>");
    client.print("<p><a href=\"/armedON\"><button class=\"button button1\">Arm</button></a></p>");
  } else {
    client.println("<p>Security system is armed</p>");
    client.print("<p><a href=\"/armedOFF\"><button class=\"button button2\">Disarm</button></a></p>");
  }

  if (*breakein == false) {
    client.println("<p>alarm is ide</p>");
    client.print("<p><a href=\"/breakeinON\"><button class=\"button button1\">turn ON </button></a></p>");
  } else {
    client.println("<p style=\"color: #FFFFFF; background-color:#FF0000;\">Breakin detekted</p>");
    client.print("<p><a href=\"/breakeinOFF\"><button class=\"button button2\">Click to turn OFF</button></a></p>");
  }

  client.println("<p>Last tag registered</p>");
  client.print("<p>");
  client.print(tag);
  client.print("</p>");

  client.println("<script>setTimeout(function(){window.location.href = '/' ;}, 5000); </script>");  //auto refresh

  client.println("</body>");
  client.println("</html>");
  client.print("\n");

  /* Close the connection */
  //client.stop();
  //Serial.println("Client disconnected");
  //Serial.print("\n");
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
      analogWrite(buzzerPin, 150);

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
