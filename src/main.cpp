#include "Sequencer.h"

// 
Ultra64 seq;

RotaryEncoder encA(ENCA_L, ENCA_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encB(ENCB_L, ENCB_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encC(ENCC_L, ENCC_R, RotaryEncoder::LatchMode::TWO03);

void setup() {
  Serial.begin(115200);
  seq.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  seq.tickReadouts();
}

