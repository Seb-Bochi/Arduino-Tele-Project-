#include "RFID.h"

void setup() {
    RFIDsetup();  // Initialize RFID module
}

void loop() {
    RFIDResult result = readUID();  // Read UID and get the result

  if (result.content != ""){
    Serial.println(result.content);
    Serial.println(result.approved);
    delay(1000);
  }


    delay(1000);  // Delay between reads
}