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

#define SLAVE_ADDRESS 0x05
int number = 0;
int state = 0;

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // Init i2c given address
  Wire.begin(SLAVE_ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready!");
}

int sensorData[10];

/*
 * Collect the latest sensor data.
 */
void collectData() {
  for (int ii = 0; ii < sizeof(sensorData); ii ++) {
    sensorData[ii] = random(-ii, ii);
  }
  return;
}

// Used for keeping time
int tickRate = 100;
float logTime = 0;

/*
 * Gather data every tick. Do not delay - this scan screw up interrupts.
 * Use smart timing instead.
 */

boolean loggerOn = false;
void loop() {
  delay(100); // temporary
}

/*
 * Read data from Pi
 */
void receiveData(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);

    if (number == 1) {
      // sendOne();
      Serial.println("I HAVE BEEN ACTIVATED");
    }
    else {
      // figure out what we want to do here
    }
  }
}

/*
 * Just send the number 1.
 */
void sendOne() {
  Wire.write(1);
}

/*
 * Send data to Raspberry Pi.
 */

// sample data
int data[] = {10, 200, 24, 123, 125, 11, 99, 34, 76, 99, 10}; 
int index = 0; 

/*
 * Send data to pi. 
 * Either sends a '1' to activate or actual data. 
 * 
 * TODO: a reset byte for index (can be upset with small errors now). 
 */
void sendData() {
  if (loggerOn) {
    Wire.write(data[index]); 
    index ++;  
    // for loop for sending data 
    if(index >= sizeof(data)/sizeof(int)){
      index = 0; 
    }
  }
  else {
    // State 0: handshake
    Wire.write(1);
    loggerOn = true;
    digitalWrite(13, LOW); 
  }
}
