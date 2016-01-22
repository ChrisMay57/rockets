#include <SD.h>
#include <SPI.h>
#include <Stepper.h>
#include <i2c_t3.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
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
 */

const int stepsPerRevolution = 2048;
boolean button_not_pressed_state = LOW;
double pull_revs = 0.3;
int stepper_speed = 40;
int num_blinks = 10; // # of times to blink
int final_delay = 1000; // final delay before moving motors

Stepper m1(stepsPerRevolution, 0, 2, 1, 3);
Stepper m2(stepsPerRevolution, 4, 6, 5, 7);

/*
 * Number of steps: 2,048
 */

// Set button pin, LED pin to output/
void setup() {
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
  Serial.println("System initializing");
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT);
  pinMode(13, OUTPUT);

  // set stepper speed
  m1.setSpeed(stepper_speed);
  m2.setSpeed(stepper_speed);

  //initSensors(); // default library; in next tab

  // SD for teensy
  // pinMode(10, OUTPUT);
  //  myFile = SD.open("test.txt", FILE_WRITE);
}

// Break only once
boolean moveMotors = true;

// Loop until button press
void loop() {
  // put your main code here, to run repeatedly:
  while (digitalRead(button_pin) == button_not_pressed_state) {
    delayMicroseconds(1);
    digitalWrite(13, HIGH);
  }
  if (moveMotors) {
    // waits for button push, then blinks 10 times in 5 seconds
    for (int ii = 0; ii < num_blinks; ii ++) {
      digitalWrite(13, HIGH);
      delay(250);
      digitalWrite(13, LOW);
      delay(250);
    }
    // sets LED to high for 1 second, then moves motors
    digitalWrite(13, HIGH);
    delay(final_delay); // 1 second
    // step each at the same time for the number of times desired
    Serial.println("Turning motors now");
    for (int ii = 0; ii < int(stepsPerRevolution * pull_revs); ii ++) {
      m1.step(-5);
      m2.step(5);
      // myFile.println(String(millis()) + "; " ); // print 10dof stuff
    }
    steppersOff();
    moveMotors = false;
  } else {
    digitalWrite(13, LOW);
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


