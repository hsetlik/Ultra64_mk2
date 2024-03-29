#pragma once
#include "Pins.h"
#include <Arduino.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>

class Ultra64
{
public:
Ultra64();
~Ultra64();
// call this in setup() to allocate everything and do setup stuff
void init();
// check the encoders and buttons, call this on a timer interrupt
void pollInputs();
// update the DAC and the gate outputs for the current state of the sequence, this also gets a timer interrupt
void updateOutputs();
private:
Adafruit_NeoPixel* pixels;
RotaryEncoder* encA;
RotaryEncoder* encB;
RotaryEncoder* encC;
long encAPos;
long encBPos;
long encCPos;
// control callback stuff
void encoderTurned(uint8_t enc, bool up);

};