// Do not remove the include below
#include "PCU_PCB_testcode.h"

int led = 13;



//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(led,OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
	digitalWrite(led, HIGH);
	delay(100);
	digitalWrite(led, LOW);
	delay(200);

}
