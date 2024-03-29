#pragma once
#include <Arduino.h>
#include <array>

#define SEQ_LENGTH 64
#define PG_LENGTH 16

#define LENGTH_MIN 30

struct Step
{
    bool gate = false;
    uint8_t midiNum = 48;
    uint8_t length = 255;
};

typedef std::array<Step, SEQ_LENGTH> Track;

struct Sequence
{
    Sequence() {}
    Track tracks[4];
    void clearTrack(uint8_t track)
    {
        for(auto& step : tracks[track])
        {
            step.gate = false;
            step.midiNum = 48;
            step.length = 255;
        }
    }
    void clearSequence()
    {
        for(uint8_t i = 0; i < 4; i++)
        {
            clearTrack(i);
        }
    }
};
// TODO: some way to stringify this to store/share it