#include <Wire.h>
#define SLAVE_ADDRESS 0x04

byte data[1200];
int index = 0; 
int state = 0;
unsigned int i = 0;
int number;

void setup() {
  Serial.begin(9600); // start serial for output
  // Init i2c given address
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(sendData);
  Wire.onReceive(receiveData);

  Serial.print("Ready");
}

void loop() {
  data[0] = i;
  i++;
  if (i < 254) {
    data[i] = (byte)random(0, 255);
  }

}


// callback for sending data

void sendData() {
  switch (state) {
    case 2:
      Wire.write(data[index]);
      ++index;
      if (index >= i - 1) {
        index = 0;
      }

      break;

    case 1:
      Wire.write(1);
      state = 2;
      break;
  }
}


void receiveData(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    if (number == 1) {
      state = 1;
    }
  }
}

