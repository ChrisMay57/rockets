
#include <i2c_t3.h>
/*
 * Raspberry Pi <-> Arduino Communication: Rocket Avionics
 * SSI 2015
 *
 * © SSI 2015
 */

/*
 * General layout of code:
 * 
 * This code is the framework of the comunication protocol
 * between the Pi and the Arduino. The Arduino is in state 0 when
 * turned on. When it recieves a 1 from the pi, it enters state 1. 
 * When it receives a read request while in state 1, it enters 
 * sends a 1 back to the pi and enters state 2. When it gets a read request 
 * in state 2, it sends the data that it has saved. If at any time it 
 * recieves a 1, it enters state 1 agian, sends the 1 back to the pi, and 
 * enters stat 2.
 * 
 * 
 * The Pi will cycle through all connected i2c addresses
 * to obtain data from multiple boards. It will log and
 * process this data.
 */

#define SLAVE_ADDRESS 0x05
int number = 0;              //Andrew wrote this Idk what it is 
int state = 0;               //This is the state, which starts at 0
boolean loggerOn = false;


// sample data
int data[21]; 
unsigned int index1 = 0; 


void setup() {
  data[0] = 20;
  pinMode(LED_BUILTIN,OUTPUT); // LED
  Serial.begin(9600); // start serial for output
  // Init i2c given address
  Wire.begin(I2C_SLAVE, SLAVE_ADDRESS, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);

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


void loop() {
  // delay(100); // temporary
  for(int jj = 1 ; jj < 21 ; jj++) {
  data[jj] = random(0,254);
  }
}

/*
 * Read data from Pi
 */
void receiveData(size_t byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);

    if (number == 1) {
      state = 1;
      Serial.println("I HAVE BEEN ACTIVATED");
    }
    else {
      // figure out what we want to do here
    }
  }
}


/*
 * Send data to Raspberry Pi.
 */


/*
 * Send data to pi. 
 * Either sends a '1' to activate or actual data. 
 * 
 * TODO: a reset byte for index1 (can be upset with small errors now). 
 */
void sendData() {
  switch(state){
    case 1:   //state 1, send a 1 back to the pi
    Wire.write(1);
    loggerOn = true;
    digitalWrite(13, LOW); 
    state = 2;
    break;

    case 2:   // state 2, send data to the pi
    Wire.write(data[index1]); 
    index1 ++;  
    // for loop for sending data 
    if(index1 >= sizeof(data)/sizeof(int)){
      index1 = 0; 
    }
    break;
    
   
  }
}
