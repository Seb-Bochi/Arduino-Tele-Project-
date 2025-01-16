#include <ESP8266WiFi.h>
#include <espnow.h>



// Callback when data is sent
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void initESPNow(){

  WiFi.mode(WIFI_STA);

  if(esp_now_init() !=0){
    Serial.println("Error initializing ESP-NOW");
      return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(onDataSent);
} 

void registerPeer(uint8_t* macAddress) {
    if (esp_now_add_peer(macAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Peer added successfully");
    }
}
 
void sendData(uint8_t* macAddress, void* data, size_t dataSize) {
    esp_now_send(macAddress, (uint8_t*)data, dataSize);
}