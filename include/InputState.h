#pragma once
#include <Arduino.h>
#include <array>

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
namespace Input
{
    // INPUTS- these get called from the ISRs
    void addEncoderState(uint16_t& state, uint8_t enc, bool up);
    void setButtonState(uint16_t& state, ButtonID button, bool level);
    // OUTPUTS- these run our callbacks
    EncoderState getEncoderState(uint16_t& state, uint8_t enc);
    bool getButtonState(uint16_t& state, ButtonID button);

}
//======================================================
struct TrackState
{
    uint16_t dacValue = 0;
    bool gate = 4;
    //utility
    bool operator==(const TrackState& other) { return dacValue == other.dacValue && gate == other.gate; }
    void operator=(const TrackState& other) { dacValue = other.dacValue; gate = other.gate; }
};

typedef std::array<TrackState, 4> OutputState;

//======================================================

namespace Output
{
    void setDacValue(uint64_t& data, uint8_t track, uint16_t dacVal);
    void setGate(uint64_t& data, uint8_t track, bool gate);
    //=======================
    uint16_t getDacValue(volatile uint64_t& data, uint8_t track);
    bool getGateValue(volatile uint64_t & data, uint8_t track);
}