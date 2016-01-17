#include <EEPROM.h>
#include "EEPROMAnything.h"

/*
 * Read/write floats from EEPROM.
 *
 */

static int FLOAT_SIZE = 2; // number of bytes
static int DATA_DELAY = 250;
unsigned int baseAddr = 0;

boolean printData = false;
int maxPoints = 200;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial Initialized");
}

long loopCount = 0;

// Collect data or say that you're done.
void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available() && !printData ) {
    printData = true;
    Serial.println("Printing Data");
    delay(500);
    for (int ii = 0; ii < maxPoints; ii ++) {
      int currentData = -1.0;
      EEPROM_readAnything(baseAddr + ii * FLOAT_SIZE, currentData);
      Serial.print("Position "); 
      Serial.print(ii); 
      Serial.print(": "); 
      Serial.println(currentData); 
    }
    Serial.println(); 
  }
  if (printData || loopCount > maxPoints) { // we are just printing, so this should be done.
    delay(2000);
  }
  else { // write the new heading variable.
    EEPROM_writeAnything(baseAddr + loopCount * FLOAT_SIZE, getHeading());
    delay(DATA_DELAY);
  }
  loopCount ++;
}

// Ultimately, this should use the 10 DOF.
int getHeading() {
  return millis() & INT_MAX;
}
