

/**
 * @file RFID.cpp
 * @author Lauge Brandt (s221215@dtu.dk)
 * @brief This program stores a number of electronic keys UIDs. 
 * @brief Using the RFID-RC522 module to read cards and keys.
 * @brief Check if the card or keys UID has access or should be denied.
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 * Using the RFID-RC522 module to read cards and keys.
 * Check if the card or keys UID has access or should be denied. 
 * 
 *  So far two values is calculated, an approved value, that can be 1 or 0, depending of access was approved or denied. 
 *  The UID of the card or key 
 * 
 * Futher work: Make the ESP8288 send the UID and approved flag to another ESP8288. 
 */
#include "RFID.h"

// Define the known UIDs
String knownUIDs[] = { "4D 02 8A 3F", "93 14 F4 E1" };

// Define the RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

/**
 * @brief function to setup the RFID module and serial communication
 * 
 */
void RFIDsetup() {
    Serial.begin(9600);  // Initialize serial communications with PC
    SPI.begin();         // Initiate SPI bus
    mfrc522.PCD_Init();  // Initiate MFRC522
    delay(4);
    Serial.println("Put the card close to the reader");
}



/**
 * @brief function to read the UID of a card and check if it is approved
 * 
 * @return RFIDResult the result of the RFID reading
 */

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
        result.content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        result.content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    result.content.toUpperCase();  // Convert the UID string to uppercase

    // Check if the UID matches any approved UIDs
    for (int i = 0; i < (sizeof(knownUIDs) / sizeof(knownUIDs[0])); i++) {
        if (result.content.substring(1) == knownUIDs[i]) {
            result.approved = 1;  // Mark as approved if a match is found
            break;
        }
    }

    if (result.approved) {
        Serial.println("Authorized access");
    } else {
        Serial.println("Access denied");
    }

    return result;
}