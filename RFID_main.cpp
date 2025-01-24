/**
 * @file RFID_main.cpp
 * @author Lauge Brandt (s221215@dtu.dk)
 * @brief file for the RFID module
 * @version 1-0
 * @date 2025-01-20
 * 
 * @copyright open source
 * 
 */
#include "RFID.h"


/**
 * @brief Arduino setup function. Initializes the RFID module.
 */
void setup() {
    RFIDsetup();  // Initialize RFID module
}


/**
 * @brief Arduino loop function. Continuously reads the UID of a card and checks if it is approved.
 */
void loop() {
    RFIDResult result = readUID();  // Read UID and get the result

  if (result.content != ""){
    Serial.println(result.content);
    Serial.println(result.approved);
    delay(1000);
  }


    delay(1000);  // Delay between reads
}