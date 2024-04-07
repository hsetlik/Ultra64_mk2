#include "InputState.h"

void Input::addEncoderState(uint16_t& data, uint8_t enc, bool up)
{
    uint16_t pos = (2 * enc);
    data = data | (1 << pos);
    if(up)
        data = data | (1 << (pos + 1));

}

void Input::setButtonState(uint16_t& data, ButtonID button, bool state)
{
    uint16_t mask = (1 << ((uint16_t)button + 6));
    data = state ? data & ~mask : data | mask;
}

EncoderState Input::getEncoderState(uint16_t& data, uint8_t enc)
{
    EncoderState state;
    uint16_t pos = 2 * enc;
    state.moved = data & (1 << pos);
    pos++;
    state.up = data & (1 << pos);
    return state;
}

bool Input::getButtonState(uint16_t& data, ButtonID button)
{
    uint16_t pos = 6 + (uint16_t)button;
    return data & (1 << pos);
}

//========================================

void Output::setDacValue(uint64_t& data, uint8_t track, uint16_t dacVal)
{
    uint64_t mask = dacVal << (track * 16);
    data = data | mask;
}

void Output::setGate(uint64_t& data, uint8_t track, bool gate)
{
    uint64_t mask = 1 << (((uint64_t)track * 16) + 14);
    data = gate ? data | mask : data & ~mask;
}

uint16_t Output::getDacValue(volatile uint64_t& data, uint8_t track)
{
    uint16_t value = (uint16_t)(data >> (track * 16));
    uint16_t mask = 1 << 14;
    return value & ~mask;
}

bool Output::getGateValue(volatile uint64_t & data, uint8_t track)
{
    uint64_t pos = (uint64_t)(track * 16) + 14;
    return data & (1 << pos);
}