#include <SD.h>
#include <SPI.h>
#include <Stepper.h>
#include <i2c_t3.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#define button_pin 23

/* SD Init info */
//File myFile;
//const int chipSelect = 4; // depends on how you wire the SD card

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

/*
 *  Basic parafoil breaking test.
 *  December 2015
 *  Authors: Andrew Milich and John Dean
 */

const int stepsPerRevolution = 2048;
boolean button_not_pressed_state = LOW;
double pull_revs = 0.3;
int stepper_speed = 40;
int num_blinks = 10; // # of times to blink
int final_delay = 1000; // final delay before moving motors
unsigned long flight_start_time;
unsigned long most_recent_log = 0;
short int log_number;

Stepper m1(stepsPerRevolution, 0, 2, 1, 3);
Stepper m2(stepsPerRevolution, 4, 6, 5, 7);

/*
 * Number of steps: 2,048
 */

// Set button pin, LED pin to output/
void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("System initializing");
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT);
  pinMode(13, OUTPUT);

  // set stepper speed
  m1.setSpeed(stepper_speed);
  m2.setSpeed(stepper_speed);

  initSensors(); // default library; in next tab

  // SD for teensy
  // pinMode(10, OUTPUT);
  //  myFile = SD.open("test.txt", FILE_WRITE);
}

// Break only once

boolean flight_begin = false;
boolean break_complete = false;
// Loop until button press
void loop() {
  // put your main code here, to run repeatedly:
  if(!flight_begin){
    digitalWrite(13, HIGH);
  }
  if (flight_begin){
    log_number = 0;
    // waits for button push, then blinks 10 times in 5 seconds
    for (int ii = 0; ii < num_blinks; ii ++) {
      digitalWrite(13, HIGH);
      delay(250);
      digitalWrite(13, LOW);
      delay(250);
    }
    digitalWrite(13, HIGH);
    flight_start_time = millis();
    Serial.println(F("Flight Starting Now"));
    while(flightTime() < 10000){
      logRoll();
      if(flightTime() > 1000 && !break_complete){
        Serial.println(F("Turning motors now"));
        for (int ii = 0; ii < int(stepsPerRevolution * pull_revs); ii ++) {
          //m1.step(-5);
          //m2.step(5);
          logRoll();
        }
        Serial.println(F("Done turning motors"));
        steppersOff();
        break_complete = true;
      }
    }
    EEPROM_writeAnything(0,log_number);
    Serial.println(F("Flight complete"));
    Serial.print(F("Number of roll entries logged:"));
    Serial.print(log_number);
    Serial.println(F(""));
    flight_begin = false;
  }
  if(digitalRead(button_pin) == HIGH) {
  flight_begin = true;
  break_complete = false;
  }
  if (Serial.available()) {
    Serial.println(F("Printing data from previous flight"));
    delay(500);
    short num_entries;
    EEPROM_readAnything(0, num_entries);
    Serial.print(num_entries);
    Serial.println(F(" entries were stored last flight"));
    for (int ii = 0; ii < num_entries; ii ++) {
      short currentData;
      EEPROM_readAnything(ii * 2, currentData);
      Serial.println(currentData); 
    }
    Serial.read();
  }
}

void steppersOff(){
  for (int ii = 0; ii < 8; ii++){
    pinMode(ii, OUTPUT);
    digitalWrite(ii, LOW);
  }
  Stepper m1(stepsPerRevolution, 0, 2, 1, 3);
  Stepper m2(stepsPerRevolution, 4, 6, 5, 7);  
  m1.setSpeed(stepper_speed);
  m2.setSpeed(stepper_speed);
}

long unsigned int flightTime(){
  return millis() - flight_start_time;
}



