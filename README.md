# Combined Pulse Position Modulation (CPPM) transmitter library for Arduino


This library provides an easy to use utility to generate a CPPM signal with an Arduino,
to be used with a CPPM compatible receiver. For example, the receiver on a quadcopter.


This library has been tested with an Arduino Leonardo. It uses TIMER3, which is not
available on the Arduino UNO. You should be able to change TIMER3 for another timer
available on the UNO.

## Usage example

```C++

#define PPM_PIN 11
#include "wcppm.h"

Wcppm wCPPM;

void setup() {  
  pinMode(PPM_PIN, OUTPUT);
  wCPPM.begin(PPM_PIN); //Start timer and cppm generation
}

void loop() {
  for(int i = 0; i < 5; i++){
    int val = ...;
    wCPPM.setChannel(i, val); //Write to channel i the value val
  }
}

```

License: MIT