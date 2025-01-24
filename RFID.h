/**
 * @file RFID.h
 * @author Lauge Brandt (s221215@dtu.dk)
 * @brief This program stores a number of electronic keys UIDs. 
 * @brief Using the RFID-RC522 module to read cards and keys.
 * @brief Check if the card or keys UID has access or should be denied.
 * @version 1.0
 * @date 2025-01-24
 * 
 * @copyright open source
 * 
 */
#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

// Structure to hold RFID results

/**
 * @brief struct to hold the RFID results
 * 
 */
struct RFIDResult {
    uint8_t approved;   // 1 if approved, 0 otherwise
    String content;     // UID of the detected card
};

// Extern declarations for knownUIDs and its size
extern String knownUIDs[];


// Declare functions
void RFIDsetup();
RFIDResult readUID();

// Declare the RFID instance as extern
extern MFRC522 mfrc522;

#endif  // RFID_H
