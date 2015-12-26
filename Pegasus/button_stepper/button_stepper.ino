#include <Stepper.h>
#define button_pin 10

/*
 *  Basic parafoil breaking test.
 *  December 2015
 */

const int stepsPerRevolution = 2048;

boolean button_not_pressed_state = LOW;
double pull_revs = 0.5;

Stepper m1(stepsPerRevolution, 0, 2, 1, 3);
Stepper m2(stepsPerRevolution, 4, 6, 5, 7);

/*
 * Number of steps: 2,048
 */

// Set button pin, LED pin to output/
void setup() {
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT);
  pinMode(13, OUTPUT);
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
    for (int ii = 0; ii < 10; ii ++) {
      digitalWrite(13, HIGH);
      delay(250);
      digitalWrite(13, LOW);
      delay(250);
    }
    // sets LED to high for 1 second, then moves motors 
    digitalWrite(13, HIGH); 
    delay(1000); // 1 second
    // step each at the same time for the number of times desired 
    for (int ii = 0; ii < int(stepsPerRevolution * pull_revs); ii ++) {
      m1.step(1);
      m2.step(1);
    }
  } else {
    digitalWrite(13, LOW);
  }
}
