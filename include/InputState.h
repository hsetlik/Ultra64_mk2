#pragma once
#include <Arduino.h>

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

//look this is weird but hear me out
struct EncoderState
{
    bool moved = false;
    bool up = false;
};
class InputState
{
private:
    uint16_t data;
public:
    InputState();
    InputState(const InputState& other)
    {
        data = other.data;
    }
    void init() {data = 0;}
    // INPUTS- these get called from the ISRs
    void addEncoderState(uint8_t enc, bool up);
    void setButtonState(ButtonID button, bool state);

    //OUTPUTS- these run the callbacks in Sequencer.cpp
    EncoderState getEncoderState(uint8_t enc);
    bool getButtonState(ButtonID button);



};

