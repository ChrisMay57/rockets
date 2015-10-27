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
 * enters stat 2.
 *
 *
 * The Pi will cycle through all connected i2c addresses
 * to obtain data from multiple boards. It will log and
 * process this data.
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

void initSensors()
{
  if (!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  if (!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while (1);
  }
}


void setup() {
  //  data[0] = 9;

  sensorData[0] = 5;
  pinMode(LED_BUILTIN, OUTPUT); // LED
  Serial.begin(9600); // start serial for output
  // Init i2c given address
  Wire.begin(I2C_SLAVE, SLAVE_ADDRESS0, I2C_PINS_18_19, I2C_PULLUP_EXT, I2C_RATE_400);
  Wire1.begin(I2C_SLAVE, SLAVE_ADDRESS1, I2C_PINS_29_30, I2C_PULLUP_EXT, I2C_RATE_400);
  //  Wire.begin(I2C_SLAVE, SLAVE_ADDRESS, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);

  Wire1.onReceive(receiveData);
  Wire1.onRequest(sendData);
  initSensors();
  Serial.println("Ready!");
}



/*
 * Collect the latest sensor data.
 */
void collectData() {
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation)) {
    /* 'orientation' should have valid .roll and .pitch fields */
    Serial.print(F("Roll: "));
    Serial.print(orientation.roll);
    sensorData[1] = orientation.roll;
    Serial.print(F("; "));
    Serial.print(F("Pitch: "));
    Serial.print(orientation.pitch);
    sensorData[2] = orientation.pitch;
    Serial.print(F("; "));
  }

  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .heading data now */
    Serial.print(F("Heading: "));
    Serial.print(orientation.heading);
    sensorData[3] = orientation.heading;
    Serial.print(F("; "));
  }

  /* Calculate the altitude using the barometric pressure sensor */
  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */
    float temperature;
    bmp.getTemperature(&temperature);
    /* Convert atmospheric pressure, SLP and temp to altitude    */
    Serial.print(F("Alt: "));
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature));
    sensorData[4] = bmp.pressureToAltitude(seaLevelPressure,
                                           bmp_event.pressure,
                                           temperature);
    Serial.print(F(" m; "));
    /* Display the temperature */
    Serial.print(F("Temp: "));
    Serial.print(temperature);
    sensorData[5] = temperature;
    Serial.print(F(" C"));
  }

  Serial.println(F(""));
  delay(1000);


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
  //  for(int jj = 1 ; jj < 10 ; jj++) {
  //  data[jj] = random(0,254);
  //  }
  collectData();
}

/*
 * Read data from Pi
 */
void receiveData(size_t byteCount) {
  while (Wire1.available()) {
    number = Wire1.read();
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
  if (state == 1) {
    Wire1.write(1);
    loggerOn = true;
    digitalWrite(13, LOW);
    state = 2;
    return;
  }
  else if (state == 2) {
    Wire1.write(sensorData[index1]);
    index1 ++;
    // for loop for sending data
    if (index1 >= sizeof(sensorData) / sizeof(int)) {
      index1 = 0;
    }
    return;
  }
}
