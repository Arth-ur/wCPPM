#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "wcppm.h"


Wcppm wPPM;

void Wcppm::begin(uint8_t pin){
	pinMode(pin, OUTPUT);
	ppm_pin = pin;
	digitalWrite(pin, LOW);
	state = LOW;
	current_channel = 0;
	
	
	cli();//stop interrupts

	//set timer3 interrupt at 2kHz
	TCCR3A = 0;// set entire TCCR3A register to 0
	TCCR3B = 0;// same for TCCR3B
	TCNT3  = 0;//initialize counter value to 0
	
	// turn on Clear Timer on Compare match mode
	TCCR3A |= (1 << WGM32);
	
	
	// Pulse de 
	OCR3A = PULSE_LENGTH_INC;
	// prescaler 8
	TCCR3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);   		//010 -> f/8 prescaler
	// enable timer compare interrupt
	TIMSK3 |= (1 << OCIE3A);
	
	sei();//allow interrupts

}

ISR(TIMER3_COMPA_vect) {
	//In any case we reset the counter
	TCNT3  = 0;
	//LOW? we do one pulse!
	if(wPPM.state == LOW){
		OCR3A = PULSE_LENGTH_INC;
		wPPM.current_channel++;
		digitalWrite(wPPM.ppm_pin, HIGH);
		wPPM.state = HIGH;
	}
	//HIGH? wait!
	else {
		if(wPPM.current_channel < 5){
			OCR3A = wPPM.channels[wPPM.current_channel];	//3000 -> 1500us
		}else{
			OCR3A = wPPM.padding;  //10000 -> 5000us
			wPPM.current_channel = -1;
		}
		digitalWrite(wPPM.ppm_pin, LOW);
		wPPM.state = LOW;
	}
}


void Wcppm::setChannel(uint8_t channel, uint16_t value){
	value = PRESCALER_CONSTANT_MUL_US * value - PULSE_LENGTH_INC;
	channels[channel] = value;
	
	computeFramePadding();
}

void Wcppm::computeFramePadding(){
	// sum channel values:
	uint16_t sum = 0;
	for (uint8_t channel = 0; channel  < CHANNEL_COUNT; channel ++){
		sum += channels[channel];
		sum += PULSE_LENGTH_INC;
	}
	padding = FRAME_LENGTH_INC - sum;
}