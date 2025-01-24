/**
 * @file senderESP.c
 * @author Sebastian Bochineck (s224014@dtu.dk)
 * @brief 
 * @version 1.0
 * @date 2025-01-16
 * 
 * @copyright open source
 * 
 */
#include <ESP8266WiFi.h>
#include <espnow.h>



// Callback when data is sent
/**
 * @brief callback function when data is sent
 * 
 * @param mac_addr mac address of the peer
 * @param sendStatus status of the send
 */
void onDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
/**
 * @brief initialize the ESP-NOW protocol
 * 
 */
void initESPNow(){

  WiFi.mode(WIFI_STA);

  if(esp_now_init() !=0){
    Serial.println("Error initializing ESP-NOW");
      return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(onDataSent);
} 


/**
 * @brief register a peer to the ESP-NOW protocol
 * 
 * @param macAddress the mac address of the peer
 */
void registerPeer(uint8_t* macAddress) {
    if (esp_now_add_peer(macAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Peer added successfully");
    }
}
 
/**
 * @brief send data to a peer
 * 
 * @param macAddress the mac address of the peer
 * @param data the data to be sent
 * @param dataSize the size of the data
 */
void sendData(uint8_t* macAddress, void* data, size_t dataSize) {
    esp_now_send(macAddress, (uint8_t*)data, dataSize);
}