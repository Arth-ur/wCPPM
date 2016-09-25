#define PPM_PIN 11
#include "wcppm.h"

Wcppm wCPPM;

void setup() {
	Serial.begin(9600);
  
	pinMode(PPM_PIN, OUTPUT);

	wCPPM.begin(PPM_PIN);
}

void loop() {
	if(Serial.available()>5*2){
    for(int i = 0; i < 5; i++){
      int val = Serial.read() * 256 + Serial.read();
      wCPPM.setChannel(i, val);
    }
	}
}






