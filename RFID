#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

// Define pins for the RFID module
#define RST_PIN 9  // Configurable, see typical pin layout above
#define SS_PIN 10  // Configurable, see typical pin layout above

// Declare the struct to hold RFID results
struct RFIDResult {
    uint8_t approved;   // flag (1 if approved, 0 otherwise)
    String content;     // UID of the registered card
};

// Declare functions
void RFIDsetup();
RFIDResult readUID();

// Declare the global variable for known UIDs
extern String knownUIDs[];

// Declare the RFID instance
extern MFRC522 mfrc522;

#endif  // RFID_H
