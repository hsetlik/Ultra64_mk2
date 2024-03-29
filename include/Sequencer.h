#pragma once
#include <Adafruit_MCP23X17.h>
#include <Adafruit_NeoPixel.h>
#include <RotaryEncoder.h>
#include <array>
#include "MCPButton.h"
#include "Pins.h"
#include "Sequence.h"

//defaults and ranges
#define TEMPO_DEFAULT 120
#define TEMPO_MIN 25
#define TEMPO_MAX 360

enum ButtonID
{
EncA,
EncB,
EncC,
C1,
C2,
C3,
C4,
PL,
PR
};
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
// ---------- Hardware -------------
Adafruit_NeoPixel* pixels;

RotaryEncoder* encA;
RotaryEncoder* encB;
RotaryEncoder* encC;
long encAPos;
long encBPos;
long encCPos;

Adafruit_MCP23X17 exp;

MCPButton* encAButton;
MCPButton* encBButton;
MCPButton* encCButton;
MCPButton* c1;
MCPButton* c2;
MCPButton* c3;
MCPButton* c4;
MCPButton* pLeft;
MCPButton* pRight;
std::array<MCPButton*, 9> buttons = {encAButton, encBButton, encCButton, c1, c2, c3, c4, pLeft, pRight};
//--------------------------------------
// ---------- State Variables ----------
uint16_t currentTempo = TEMPO_DEFAULT;
uint8_t currentStep = 0;
uint8_t selectedTrack = 0;
uint8_t selectedStep = 0;
bool isPlaying = false;
bool quarterMode = false;
bool lengthMode = false;
Sequence seq;
//--------------------------------------
Step& getSelected() { return seq.tracks[selectedTrack][selectedStep]; }
// control callback stuff-----------------------
void encoderTurned(uint8_t enc, bool up);
void buttonPressed(ButtonID id);
void buttonHeld(ButtonID id);

void shiftTempo(bool dir);
void shiftSelectedStep(bool dir);
void shiftStepNote(bool dir);
void shiftStepLength(bool dir);
//--------------------------------------

};