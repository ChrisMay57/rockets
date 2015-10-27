#include <i2c_t3.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

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
 * enters state 2.
 *
 *
 * The Pi will cycle through all connected i2c addresses
 * to obtain data from multiple boards. It will log and
 * process this data.
 * 
 * States: 
 *    1: not active
 *    2: sending and tracking data 
 *    3: sending test data to reset pi location 
 * 
 * Change Log: 
 */

#define SLAVE_ADDRESS0 0x05
#define SLAVE_ADDRESS1 0x07

int number = 0;              //Andrew wrote this Idk what it is
int state = 0;               //This is the state, which starts at 0
boolean loggerOn = false;


// sample data
//int data[10];
//actual data
int sensorData[6];
unsigned int index1 = 0;

void setup() {
  //  data[0] = 9;

  sensorData[0] = 5;
  pinMode(LED_BUILTIN, OUTPUT); // LED
  Serial.begin(9600); // start serial for output
  
  // Init i2c given address
  Wire.begin(I2C_SLAVE, SLAVE_ADDRESS, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
  Wire1.begin(I2C_SLAVE, SLAVE_ADDRESS1, I2C_PINS_29_30, I2C_PULLUP_EXT, I2C_RATE_400);
  //  Wire.begin(I2C_SLAVE, SLAVE_ADDRESS, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  initSensors();
  Serial.println("Ready!");
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
  //  for(int jj = 1 ; jj < 10 ; jj++) {
  //  data[jj] = random(0,254);
  //  }
  delay(100); 
}

/*
 * Read data from Pi
 */
void receiveData(size_t byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);

    state = number; 
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
int testData[6] = {1,1,1,1,1,1};
 
void sendData() {
  if (state == 1) {
    Wire.write(1);
    loggerOn = true;
    digitalWrite(13, LOW);
    state = 2;
    return;
  }
  else if (state == 2) {
    Wire.write(sensorData[index1]);
    index1 ++;
    // for loop for sending data
    if (index1 >= sizeof(sensorData) / sizeof(int)) {
      index1 = 0;
    }
    collectData();
    return;
  }
  else if (state == 3){
    Wire.write(1);
  }
}
