/* V1.0. RFID_code
 * This program stores a number of electronic keys UIDs. 
 * Using the RFID-RC522 module to read cards and keys.
 * Check if the card or keys UID has access or should be denied. 
 *
 * So far two values is calculated, an approved value, that can be 1 or 0, depending of access was approved or denied. 
 * The UID of the card or key 
 *
 * Futher work: Make the ESP8288 send the UID and approved flag to another ESP8288. 


*/
#include "RFID.h"
#include <Arduino.h>

void RFIDsetup() {

   
    SPI.begin();         // Initiate SPI bus
    mfrc522.PCD_Init();  // Initiate MFRC522
    delay(4);
}

RFIDResult readUID() {
    RFIDResult result;  // Create a struct to hold the result
    result.content = "";
    result.approved = 0;

    if (!mfrc522.PICC_IsNewCardPresent()) {
        return result;  // Return empty result if no card is present
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return result;  // Return empty result if UID can't be read
    }

    // Construct the UID string
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        result.content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        result.content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    result.content.toUpperCase();  // Convert the UID string to uppercase

    // Check if the UID matches any approved UIDs
    for (int i = 0; i < (sizeof(knownUIDs) / sizeof(knownUIDs[0])); i++) {
        if (result.content == knownUIDs[i]) {
            result.approved = 1;  // Mark as approved if a match is found
            break;
        }
    }

    return result;
}