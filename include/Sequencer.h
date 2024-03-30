#pragma once
#include "Pins.h"
#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP4728.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
#include <FastLED.h>
#include <array>
#include <Wire.h>
#include "MCPButton.h"
#include "Sequence.h"

// defaults and ranges
#define TEMPO_DEFAULT 120
#define TEMPO_MIN 25
#define TEMPO_MAX 360

#define OUTPUT_UPDATE_HZ 1000

#define HALFSTEP_INCREMENT 26.167f

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

struct MessageBuffer
{
private:
    String messages[16];
    uint8_t head;
public:
    MessageBuffer() : head(0) {}
    void push(const String& msg)
    {
        messages[head] = msg;
        head = (head + 1) % 16;
    }
    String& operator[](uint8_t idx)
    {
        uint8_t i = (head + idx) % 16;
        return messages[i];
    }

};



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
    // update the DAC and the gate outputs for the current state of the sequence, this also gets a timer interrupt at OUTPUT_UPDATE_HZ
    void updateOutputs();

private:
    // ---------- Hardware -------------
   // Adafruit_NeoPixel *pixels;
   CRGB pixels[24];

    Adafruit_SSD1306* display;
    MessageBuffer log;

    RotaryEncoder *encA;
    RotaryEncoder *encB;
    RotaryEncoder *encC;
    long encAPos;
    long encBPos;
    long encCPos;

    Adafruit_MCP23X17 exp;

    MCPButton *encAButton;
    MCPButton *encBButton;
    MCPButton *encCButton;
    MCPButton *c1;
    MCPButton *c2;
    MCPButton *c3;
    MCPButton *c4;
    MCPButton *pLeft;
    MCPButton *pRight;
    std::array<MCPButton *, 9> buttons = {encAButton, encBButton, encCButton, c1, c2, c3, c4, pLeft, pRight};

    Adafruit_MCP4728 dac;
    // helper function for the gate outputs
    void setGate(uint8_t channel, bool level)
    {
        switch (channel)
        {
        case 0:
            exp.digitalWrite(CH1, !level);
            break;
        case 1:
            exp.digitalWrite(CH2, !level);
            break;
        case 2:
            exp.digitalWrite(CH3, !level);
            break;
        case 3:
            exp.digitalWrite(CH4, !level);
            break;
        default:
            break;
        }
    }
    void setCV(uint8_t channel, uint8_t midiNote)
    {
        uint16_t value = (uint16_t)((float)midiNote * HALFSTEP_INCREMENT);
        dac.setChannelValue((MCP4728_channel_t)channel, value, MCP4728_VREF_INTERNAL);
    }
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
    Step &getSelected() { return seq.tracks[selectedTrack][selectedStep]; }
    // control callback stuff-----------------------
    void encoderTurned(uint8_t enc, bool up);
    void buttonPressed(ButtonID id);
    void buttonHeld(ButtonID id);

    void shiftTempo(bool dir);
    void shiftSelectedStep(bool dir);
    void shiftStepNote(bool dir);
    void shiftStepLength(bool dir);
    //--------------------------------------
    //--------helpers for the readouts------
    CRGB noteOnColors[12];
    CRGB noteSelectedColors[12];

    const CHSV minLengthColor;
    const CHSV maxLengthColor;
    const CRGB offColor;

    void updateDisplay();
    void updatePixels();
    //--------------------------------------
    // helpers for timing math
    const float sampleIntervalMs = 1000.0f / (float)OUTPUT_UPDATE_HZ;
    float stepLengthMs = 60000.0f / (float)TEMPO_DEFAULT;
    float msIntoSequence = 0.0f;
    float msIntoStep = 0.0f;
    float noteLengthMs(uint8_t length);
};