#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

// Structure to hold RFID results
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
