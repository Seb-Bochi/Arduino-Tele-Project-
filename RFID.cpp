/* RFID_code
 * This code is resposible for handling the initialization and operation of the RFID module.
 * It can detect, read and validate tags using the MFRC522 RFID reader.
  
 * Dependencies:
 * - The code depends on the MFRC522 library for interacting with the RFID reader.
 * - It also uses the Arduino `SPI` library for communication with the RFID module.
*/


#include "RFID.h"
#include <Arduino.h>


/* 
 ** RFIDsetup**
 * This function handles the initialization of the MFRC522 module. 
*/

void RFIDsetup() {
 
    SPI.begin();         // Initiate SPI bus
    mfrc522.PCD_Init();  // Initiate MFRC522
    delay(4);
}

/* 
 ** RFIDResult readUID**
 * This function reads the UID of a detected RFID tag. 
 * The UID is constructed as a hexadecimal.  
 * The detected UID is comapred against a predefined list of UIDs
 * If a match is found the approved field of return is set to 1. 
 * The UID is stored in the content field of return as a String. 
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