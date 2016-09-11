// Do not remove the include below
#include "PCU_PCB_testcode.h"



enum {
	EVENT_UPDATE = 0x01,
	EVENT_BUZZER = 0x02,
};
IntervalTimer main_loop_timer;
IntervalTimer buzzer_timer;
volatile uint16_t events;

const int led = 13;
const int chipSelect = 10;
byte led_counter;
bool buzzer_toggle;

Sd2Card card;
SdVolume volume;
SdFile root;

Adafruit_BMP280 bmp(BMP1_CS);



void eventChecker(void);
void delayWithBeeps(int n_beeps);
void sdCheck();

//The setup function is called once at startup of the sketch
void setup() {
	pinMode(LED_1, OUTPUT);
	pinMode(LED_2, OUTPUT);
	pinMode(LED_3, OUTPUT);
	pinMode(LED_4, OUTPUT);
	pinMode(BUZZER, OUTPUT);

	pinMode(ETRIG1, OUTPUT);
	pinMode(ETRIG2, OUTPUT);
	digitalWrite(ETRIG1, 0);
	digitalWrite(ETRIG2, 0);
	pinMode(ESENSE1, INPUT);
	pinMode(ESENSE2, INPUT);

	Serial.begin(9600);
	delayWithBeeps(3);
	sdCheck();
	main_loop_timer.begin(eventChecker, 100000);
	main_loop_timer.priority(0);

}

// The loop function is called in an endless loop
void loop() {

	if (events & EVENT_UPDATE){
		//digitalWrite(LED_1, led_counter == 0);
		//digitalWrite(LED_2, led_counter == 1);
		digitalWrite(LED_3, led_counter == 2);
		digitalWrite(LED_4, led_counter == 3);
		led_counter++;
		if(led_counter > 3){led_counter = 0;}
		//digitalWrite(ETRIG1, led_counter == 0);
		//digitalWrite(ETRIG2, led_counter == 0);
		if(digitalRead(ESENSE1) == HIGH){
			digitalWrite(LED_1, 1);
			digitalWrite(ETRIG2, 1);
		} else {
			digitalWrite(LED_1, 0);
			digitalWrite(ETRIG2, 0);
		}


		if(digitalRead(ESENSE2) == HIGH){
			digitalWrite(LED_2, 1);
		} else {
			digitalWrite(LED_2, 0);
		}



//		if (!bmp.begin()) {
//			Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
//		}
//		sdCheck();





		events &= ~EVENT_UPDATE;


	}

	if (events & EVENT_BUZZER){

		events &= ~EVENT_BUZZER;
	}
}

void eventChecker(void) {
	events |= EVENT_UPDATE;

	//events |= EVENT_BUZZER;
}

void sdCheck(){
	  Serial.print("\nInitializing SD card..");
	  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
	    Serial.println("initialization failed. Things to check:");
	    Serial.println("* is a card inserted?");
	    Serial.println("* is your wiring correct?");
	    Serial.println("* did you change the chipSelect pin to match your shield or module?");
	    return;
	  } else {
	   Serial.println("Wiring is correct and a card is present.");
	  }

	  // print the type of card
	  Serial.print("\nCard type: ");
	  switch(card.type()) {
	    case SD_CARD_TYPE_SD1:
	      Serial.println("SD1");
	      break;
	    case SD_CARD_TYPE_SD2:
	      Serial.println("SD2");
	      break;
	    case SD_CARD_TYPE_SDHC:
	      Serial.println("SDHC");
	      break;
	    default:
	      Serial.println("Unknown");
	  }

	  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	  if (!volume.init(card)) {
	    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
	    return;
	  }


	  // print the type and size of the first FAT-type volume
	  uint32_t volumesize;
	  Serial.print("\nVolume type is FAT");
	  Serial.println(volume.fatType(), DEC);
	  Serial.println();

	  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	  if (volumesize < 8388608ul) {
	    Serial.print("Volume size (bytes): ");
	    Serial.println(volumesize * 512);        // SD card blocks are always 512 bytes
	  }
	  Serial.print("Volume size (Kbytes): ");
	  volumesize /= 2;
	  Serial.println(volumesize);
	  Serial.print("Volume size (Mbytes): ");
	  volumesize /= 1024;
	  Serial.println(volumesize);

}

void delayWithBeeps(int n_beeps){
	while(n_beeps > 0){
		digitalWrite(BUZZER, 1);
		delay(100);
		digitalWrite(BUZZER, 0);
		delay(100);
		n_beeps--;
	}
}
