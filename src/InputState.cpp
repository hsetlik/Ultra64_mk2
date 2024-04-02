#include "InputState.h"
InputState::InputState() : data(0)
{

}

void InputState::addEncoderState(uint8_t enc, bool up)
{
    uint16_t pos = (2 * enc);
    data = data | (1 << pos);
    if(up)
        data = data | (1 << (pos + 1));
}

void InputState::setButtonState(ButtonID id, bool state)
{
    uint16_t mask = (1 << ((uint16_t)id + 6));
    data = state ? data & ~mask : data | mask;
}

EncoderState InputState::getEncoderState(uint8_t enc)
{
    EncoderState state;
    uint16_t pos = 2 * enc;
    state.moved = data & (1 << pos);
    pos++;
    state.up = data & (1 << pos);
    return state;
}

bool InputState::getButtonState(ButtonID button)
{
    uint16_t pos = 6 + (uint16_t)button;
    return data & (1 << pos);
}