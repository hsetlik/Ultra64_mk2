#pragma once
#include <Arduino.h>
#include <functional>

// timing constants

// the core concepts here are based on this arduino library: https://github.com/mathertel/OneButton

typedef std::function<void()> ButtonCallback;

namespace Btn
{
    const unsigned long debounceMs = 50;
    const unsigned long clickMs = 350;
    const unsigned long pressMs = 600;
    const unsigned long idleMs = 1000;
}

enum BtnState : int
{
    INIT,
    DOWN,
    UP,
    COUNT,
    PRESS,
    PRESSEND
};

class Button
{
private:
    const bool activeLevel;

    // button state stuff
    BtnState state = INIT;
    bool debouncedLevel = !activeLevel;
    bool lastDebounceLevel = !activeLevel;
    unsigned long lastDebounceTime = 0;
    unsigned long now = 0;

    unsigned long startTime = 0;
    int numClicks = 0;
    int maxClicks = 1;

    uint32_t pressIntervalMs = 850;
    unsigned long lastDuringPressTime = 0;
    
    // callbacks
    ButtonCallback onClick;
    bool hasOnClick = false;

    ButtonCallback onDoubleClick;
    bool hasOnDoubleClick = false;

    ButtonCallback onPressStart;
    bool hasOnPressStart = false;

    ButtonCallback onPressStop;
    bool hasOnPressStop = false;

    ButtonCallback duringPress;
    bool hasDuringPress = false;
//-------------------------------------------
    void reset();
    bool debounce(bool lvl);

    
    // runs our finite state machine w/ the BtnState enum
    void fsm(bool lvl);
    
public:
    Button(bool activeLow=true);

    // call this at a given interval and pass the value
    // from the expander
    void tick(bool lvl);

    void attachOnClick(ButtonCallback func)
    {
        onClick = func;
        hasOnClick = true;
    }

    void attachOnDoubleClick(ButtonCallback func)
    {
        onDoubleClick = func;
        hasOnDoubleClick = true;
        maxClicks = 2;
    }

    void attachOnPressStart(ButtonCallback func)
    {
        onPressStart = func;
        hasOnPressStart = true;
    }

    void attachOnPressStop(ButtonCallback func)
    {
        onPressStop = func;
        hasOnPressStop = true;
    }

    void attachDuringPress(ButtonCallback func)
    {
        duringPress = func;
        hasDuringPress = true;
    }
};