#include <Wire.h>
#define SLAVE_ADDRESS 0x04

long data[] = {5,100,24,123,125,11};
int index = 0;
void setup() {
    Serial.begin(9600); // start serial for output
  // Init i2c given address
    Wire.begin(SLAVE_ADDRESS);
    Wire.onRequest(sendData); 
    
    Serial.print("Ready");
}

void loop() {
}


// callback for sending data

void sendData() { 
    Wire.write(data[index]);
    ++index;
    if (index >= 6) {
         index = 0;
    }
 }
