/*
  Inertial Data SD card logger + statistics 
 
 Developed for the Stanford Student Space Initiative Tier 3 rocket team, "Lightning" rocket program. 
 Prints pitch, roll, heading, distributed temperatures, and light environment to serial and to Datafile. 
 Built on libraries and examples provided by Adafruit. 
 
 Hardware:
 * Adafruit Ultimate GPS + Logging Shield 
 ** Adafruit 10DOF Brakout Board
 *** SSI custom sensor interface printed circuit board (PCB)
 **** 2x analog temperature sensors + 1x analog photodiode 
 
 Troubleshooting: 
 * SD card attached to SPI bus as follows:
   UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
 ** ensure SDA/SCL connections are made between the sensor interface PCB and the arduino
 *** When uploading new code: if the arduino compiler complains that the serial port is currently
     in use, try pressing the reset button on the arduino while uploading. It may take a few attempts
 **** Ensure you are using the most recent Adafruit sensor libraries before compiling 
      (https://learn.adafruit.com/adafruit-10-dof-imu-breakout-lsm303-l3gd20-bmp180/software)
 
 created  12 Feb 2015 
 modified 26 Aug 2015 by Cameron Ramos 
 	 
 AS OF 07-17-2015, THIS CODE MEETS DATA LOGGING REQUIREMENTS FOR SSI LIGHNING
 ALL MODIFICATIONS MADE BEYOND THIS DATE ARE LISTED BELOW:
   - Updated error function to print to both datafile and serial 
   - Added reset function to improve chances of reconnecting to the interface board in case of
     unintended sda/scl disconnect
 
 */

#include <SoftwareSerial.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_10DOF.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

#define chipSelect 10
#define ledPin 13
#define aref_voltage 3.3 

/* Analog sensor pin assignments and raw value variables */
int photocellPin = 0; // the cell and 10K pulldown are connected to a0
int pinNose = 1;  //analog pin for nose temp sensor
int tempReadingN; //nose temperature reading
int pinPay = 2; //analog pin for payload temp sensor
int tempReadingP; //payload temperature reading
int photocellReading;  //raw photocell reading


/* OFFSETS */
int rollOff = 0;
int pitchOff = 0;

/* Array's and sum variables needed to perform average/standard dev. functions */
float rolling[5] = {0,0,0,0,0};
float pitching[5] = {0,0,0,0,0};
float rollSum = 0;
float pitchSum = 0;
float headSum = 0;

float temperatures[5] = {0,0,0,0,0};
float pressures[5] = {0,0,0,0,0};
float tempSum = 0;
float pressSum = 0;

float payTempSum = 0;
float noseTempSum = 0;

/* output string for Big Red Bee radio (currently not implemented) */
char output[18];

int looping = 0; //variable to allow standard deviation to be produced every 5 seconds

RTC_DS1307 RTC;
File dataFile;

//------------------------------initSensors------------------------------//
/* Initializes sensors and produces warnings when sensors are not connected
 *
 */
void initSensors()
{
  Serial.print(F("Initializing sensors... "));
  dataFile.print("Initializing sensors... ");
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    error("No LSM303 detected");
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    error("No LSM303 detected");
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    error("No BMP180 detected");
  }
  Serial.println("COMPLETE");
  dataFile.println("COMPLETE");
}

//------------------------------printTime------------------------------//
void printTime(){
  DateTime now = RTC.now();
  dataFile.println();
  dataFile.print(now.year(), DEC);
  dataFile.print('/');
  dataFile.print(now.month(), DEC);
  dataFile.print('/');
  dataFile.print(now.day(), DEC);
  dataFile.print(' ');
  dataFile.print(now.hour(), DEC);
  dataFile.print(':');
  dataFile.print(now.minute(), DEC);
  dataFile.print(':');
  dataFile.print(now.second(), DEC);
  dataFile.println();
}

//------------------------------resetFunc-----------------------------------//
void(* resetFunc) (void) = 0;//declare reset function at address 0

//------------------------------error-----------------------------------//
void error(char *str)
{
  Serial.print("error: "); Serial.println(str);
  dataFile.print("error: "); dataFile.println(str);
  Serial.println(""); Serial.println("ATTEMPTING RESET..."); Serial.println("");
  dataFile.println(""); dataFile.println("ATTEMPTING RESET..."); dataFile.println("");
  
  dataFile.flush();
  delay(500);
  resetFunc();
}

//------------------------------initSDcard------------------------------//
void initSDCard(){
  // initialize the SD card
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(pinPay, INPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(10, 11, 12, 13)) {
    /*cannot use the error function here since dataFile is not initialized */
    Serial.println("Card failed, or not present"); 
    while(1);
  }
  Serial.println("card initialized.");
}  

//------------------------------initLoggingFile------------------------------//
/* UNIMPLEMENTED FUNCTION TO CREATE NEW LOG FILES ON EACH POWER ON/OFF

File logfile;

void initLoggingFile(){
  // create a new file
  char filename[] = "datalog00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = i/10 + '0';
    filename[8] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  if (! logfile) {
    error("couldnt create file");
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
}
*/

//------------------------------SETUP------------------------------//
void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("SSI LIGHNING Data Logging")); 
  Serial.println("");
  Wire.begin();
  
  /* Initialize the SD card*/
  initSDCard();

  /* Open log file*/
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error: could not open datalog.txt");
    while (1); /* Wait forever since we cant write data */
  }
  RTC.begin();
  
  dataFile.println("\n");
  dataFile.println("——————————————————————————————————————————————");
  dataFile.println("-------------Starting New Session-------------");
  printTime(); /* indicate start of new session */
  initSensors(); /* Initialise the sensors */
  dataFile.println("——————————————————————————————————————————————");

  analogReference(EXTERNAL);
}

//------------------------------LOOP------------------------------//
/* Constantly checks the roll/pitch/heading/altitude/temperature. 
 * Prints to Serial and dataFile.
 */
void loop()
{
  printTime();
  
  //inertialStats(looping);
  inertialStats2(looping);
  bmpStats(looping);
  distTemp(looping);
  light(looping);


  looping += 1;
  if(looping > 4){
    looping = 0;
  }

  dataFile.println();
  dataFile.flush();
  delay(1000);
}


//------------------------------inertialStats2------------------------------/
/* Calls newer library functions to read accelerometer and gyroscopic data
 * from the digital Adafruit 10DOF board. Prints these values along with average 
 * and standard deviation calculations every 5 seconds. 
 */
void inertialStats2(int looping){
  sensors_event_t accel_event;
  sensors_vec_t   orientation;
  sensors_event_t mag_event;
  sensors_event_t gyro_event;      
  
  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);
  gyro.getEvent(&gyro_event);

  /* Use the new fusionGetOrientation function to merge accel/mag data */  
  if (dof.fusionGetOrientation(&accel_event, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    int roll = orientation.roll - rollOff;
    int pitch = orientation.pitch - pitchOff;
    
    dataFile.print(F("Orientation --> "));
    dataFile.print("Roll: "); dataFile.print(roll);
    dataFile.print("; Pitch: "); dataFile.print(pitch);
    // dataFile.print("; Heading: "); dataFile.print(orientation.heading); PRODUCES NEGATIVE VALUES
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
    {
      dataFile.print("; Heading: "); dataFile.print(orientation.heading);
      headSum += orientation.heading;
    }
    dataFile.println(F(""));
    /* REMOVED SPIN DATA -- FAULTY DURING TESTING */
    /*dataFile.print("Spin --> ");
    dataFile.print("X: "); dataFile.print(gyro_event.gyro.x); dataFile.print("  ");
    dataFile.print("Y: "); dataFile.print(gyro_event.gyro.y); dataFile.print("  ");
    dataFile.print("Z: "); dataFile.print(gyro_event.gyro.z); dataFile.print("  ");
    dataFile.println("rad/s ");*/
 
    rolling[looping] = roll;
    pitching[looping] = pitch;

    rollSum += roll;
    pitchSum += pitch;

    if(looping == 4){
      float rollAve = rollSum/5;
      float pitchAve = pitchSum/5;
      
      float headAve = headSum/5;
      float rolldiff = 0;
      float pitchdiff = 0;

      for(int k = 0; k<5; k++){
        rolldiff += pow(abs(rolling[k] - rollAve),2);        
        pitchdiff += pow(abs(pitching[k] - pitchAve),2);
      }

      float rollDev = sqrt((rolldiff / 5));
      float pitchDev = sqrt((pitchdiff / 5));
      
      dataFile.println("-------IMU STATS-------");
      dataFile.print("Roll Ave: "); dataFile.print(rollAve);
      dataFile.print("; SD: "); dataFile.print(rollDev);
      dataFile.print("; Pitch Ave: "); dataFile.print(pitchAve);
      dataFile.print("; SD: "); dataFile.println(pitchDev);
      dataFile.println("-------DONE-------");
      dataFile.println();
      
      Serial.print(F("Roll Ave: ")); Serial.print(rollAve);
      Serial.print(F("; SD: ")); Serial.print(rollDev);
      Serial.print(F("; Pitch Ave: ")); Serial.print(pitchAve);
      Serial.print(F("; SD: "));  Serial.print(pitchDev);
      Serial.print(F(" ; Heading: ")); Serial.print(headAve); Serial.println(F(" "));

      rollSum = 0; 
      pitchSum = 0;
      headSum = 0;
    }
    
    dataFile.print("Acceleration --> ");
    /* Display the results (acceleration is measured in m/s^2) */
    dataFile.print("X: "); dataFile.print(accel_event.acceleration.x); dataFile.print("  ");
    dataFile.print("Y: "); dataFile.print(accel_event.acceleration.y); dataFile.print("  ");
    dataFile.print("Z: "); dataFile.print(accel_event.acceleration.z); dataFile.print("  ");dataFile.println("m/s^2 ");
  } 
}


//------------------------------bmpStats------------------------------//
/* Calls library functions to read barometric pressure and temperature data
 * from the digital Adafruit 10DOF board. Prints these values along with an
 * estimated pressure based altitude. Also makes average and standard deviation
 * calculations every 5 seconds. 
 */
void bmpStats(int looping){
  /* Calculate the altitude using the barometric pressure sensor */
  sensors_event_t bmp_event;
  bmp.getEvent(&bmp_event);

  if (bmp_event.pressure){
    /* Get ambient temperature in C */
    float temperature;
    bmp.getTemperature(&temperature);
    float pressure = bmp_event.pressure;
    temperature = (temperature * 9/5) + 32; //convert to Farenheit

    dataFile.print("Temperature/Pressure --> ");
    dataFile.print(" Pressure: ");
    dataFile.print(pressure);
    dataFile.print(" hP; Chip Temp: ");
    dataFile.print(temperature);
    dataFile.print(" F; ");
    dataFile.print(F("Alt: "));
    dataFile.print(bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature)); 


    temperatures[looping] = temperature;
    pressures[looping] = pressure;

    tempSum += temperature; 
    pressSum += pressure;

    if(looping == 4){

      float pressAve = pressSum/5;
      float tempAve = tempSum/5;

      float tempDiff = 0;
      float pressDiff = 0;

      for(int k = 0; k < 5; k++){
        pressDiff += pow((pressures[k] - pressAve),2);
        tempDiff += pow((temperatures[k] - tempAve),2);
      }

      float pressDev = sqrt(pressDiff / 5);
      float tempDev = sqrt(tempDiff / 5);

      Serial.print(F("Pressure: ")); 
      Serial.print(pressAve);
      Serial.print(F(" hPascals; "));
      Serial.print(F("SD: "));
      Serial.print(pressDev);
      Serial.println(" ");
      Serial.print(F("Chip Temp: "));
      Serial.print(tempAve);
      Serial.print(F(" F ")); 
      Serial.print(F("SD: "));
      Serial.print(tempDev);
      Serial.println(" ");

      tempSum = 0;
      pressSum = 0;
    }

  }
}

//------------------------------distTemp------------------------------//
/* Reads in raw inputs from the nose cone and payload distributed
 * temperature sensors and calls farenheight() t convert the raw voltage to
 * degrees.
 */
void distTemp(int looping){
  float nraw = analogRead(pinNose);
  float praw = analogRead(pinPay);

  // converting that reading to voltage, which is based off the reference voltage
  float nvoltage = nraw * aref_voltage / 1024; 
  float pvoltage = praw * aref_voltage / 1024;

  // converting to farenheit
  float ntemp = farenheit(nvoltage);
  float ptemp = farenheit(pvoltage);

  payTempSum += ptemp;
  noseTempSum += ntemp;

  dataFile.print("; Nose temp: "); 
  dataFile.print(ntemp); 
  dataFile.print(" F; ");
  dataFile.print("Payload temp: "); 
  dataFile.print(ptemp); 
  dataFile.print(" F; ");

  if(looping == 4){
    float ntemp = noseTempSum/5;
    float ptemp = payTempSum/5;

    Serial.print(F("Nose temp: ")); 
    Serial.print(ntemp); 
    Serial.print(F(" F; "));
    Serial.print(F(" Payload temp: ")); 
    Serial.print(ptemp); 
    Serial.print(F(" F; "));

    payTempSum = 0;
    noseTempSum = 0;
  } 
}

//------------------------------light------------------------------//
/* Reads raw input on the photocellPin and prints out an unsclaed
 * light intensity value from 0 to 1024 (no light to maximum detectable 
 * light) along with a description of lighting conditions. This ranges
 * for each description can be calibrated to indicate different lighting 
 * conditions (ie. nose cone is attached/seperated)
 */
void light(int looping){
  photocellReading = analogRead(photocellPin);  
  
  dataFile.print("Light reading = ");
  dataFile.print(photocellReading);

  if (photocellReading < 200) {
    dataFile.print(" -- Dark; ");
  } 
  else if(photocellReading < 600) {
    dataFile.print(" -- Dim; ");
  }
  else {
    dataFile.print(" -- Light; ");
  }

  if(looping == 4){
    Serial.print(F("Light reading = "));
    Serial.print(photocellReading);
    if (photocellReading < 200) {
      Serial.println(F(" - Dark; "));
    } 
    else if(photocellReading < 600) {
      Serial.print(" -- Dim; ");
    }
    else {
      Serial.println(F(" - Light; "));
    }
  }

}

//------------------------------farenheit------------------------------//
/* Converts raw voltage values from distributed temperature sensors to farenheit.  
 *
 */
float farenheit(float volts){
  float temperatureC = (volts - 0.5) * 100;
  float temperatureF = (temperatureC * 9 / 5) + 32;
  return temperatureF;
}

//------------------------------inertialStats------------------------------//
/* Unused function based on older Adafruit libraries. Not called in loop(). 
 *
 */
void inertialStats(int looping){
  sensors_event_t accel_event;
  sensors_vec_t   orientation;
  sensors_event_t mag_event;

  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation)){

    float roll = orientation.roll; 

    dataFile.print("Roll: ");
    dataFile.print(roll);
    dataFile.print("; Pitch: ");
    float pitch = orientation.pitch;
    dataFile.print(pitch);
    dataFile.print("; ");


    rolling[looping] = roll;
    pitching[looping] = pitch;

    rollSum += roll;
    pitchSum += pitch;

    if(looping == 4){
      float rollAve = rollSum/5;
      float pitchAve = pitchSum/5;
      float rolldiff = 0;
      float pitchdiff = 0;

      for(int k = 0; k<5; k++){
        rolldiff += pow(abs(pitching[k] - rollAve),2);
        pitchdiff += pow(abs(rolling[k] - pitchAve),2);
      }

      float rollDev = sqrt(rolldiff / 5);
      float pitchDev = sqrt(pitchdiff / 5);
      
      dataFile.println();
      dataFile.println("-------IMU STATS-------");
      
      Serial.print(F("Roll Ave: "));
      dataFile.print("Roll Ave: ");
      Serial.print(rollAve);
      dataFile.print(rollAve);
      Serial.print(F("; SD: "));
      dataFile.print("; SD: ");
      Serial.print(rollDev);
      dataFile.print(rollDev);
      Serial.print(F("; Pitch Ave: "));
      dataFile.print("; Pitch Ave: ");
      Serial.print(pitchAve);
      dataFile.print(pitchAve);
      Serial.print(F("; SD: "));
      dataFile.print("; SD: ");
      Serial.print(pitchDev);
      dataFile.println(pitchDev);
      Serial.print(F(" ;"));
      dataFile.println("-------DONE-------");
      dataFile.println();

      rollSum = 0; 
      pitchSum = 0;
    } 
  }   

  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {

    float heading = orientation.heading;
    dataFile.print("Heading: ");
    dataFile.print(heading);
    
    headSum += heading;

    if(looping == 4){
      float headAve = headSum/5;
      Serial.print(F("Heading: "));
      Serial.print(headAve);
      Serial.print(F(" "));
      headSum = 0;
    } 
  }
}


