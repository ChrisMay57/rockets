#include <Wire.h>

/*
 * Raspberry Pi <-> Arduino Communication: Rocket Avionics 
 * SSI 2015 
 * 
 * © SSI 2015
 */

 /*
  * General layout of code: 
  * 
  * The Arduino constantly updates sensors in the loop. 
  * The Pi communicates via i2c to the arduino; the Wire 
  * library triggers an interrupt that will send the latest 
  * data to the Arduino. The Arduino will return a message
  * confirming receipt of the data. 
  * 
  * The Pi will cycle through all connected i2c addresses 
  * to obtain data from multiple boards. It will log and 
  * process this data. 
  */

  /*
   * TODO: 
   * 
   * * add tutorial credit link!!! 
   */

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}
  
void loop() {
  delay(100); // Andrew: the sensors should be updated here into global variables. 
  // When the PI triggers the i2c 'receive' or 'send' data interrupts, the Arduino 
  // sends a message. 
}

// callback for received data
void receiveData(int byteCount) {

  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);

    if (number == 1) {

      if (state == 0) {
        digitalWrite(13, HIGH); // set the LED on
        state = 1;
      }
      else {
        digitalWrite(13, LOW); // set the LED off
        state = 0;
      }
    }
  }
}

// callback for sending data
void sendData() {
  Wire.write(number);
}
