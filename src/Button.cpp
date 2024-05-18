#include "Button.h"



//-------------------------------------------------------

Button::Button(bool activeLow) : activeLevel(!activeLow)
{

}

void Button::reset()
{
    state = BtnState::INIT;
    numClicks = 0;
    startTime = millis();
}

bool Button::debounce(bool lvl)
{
    now = millis();
    if(lastDebounceLevel == lvl)
    {
        if(now - lastDebounceTime >= Btn::debounceMs)
            debouncedLevel = lvl;
    }
    else
    {
        lastDebounceTime = now;
        lastDebounceLevel = lvl;
    }
    return debouncedLevel;
}

void Button::tick(bool lvl)
{
    fsm(debounce(lvl) == activeLevel);
}

void Button::fsm(bool active)
{
    unsigned long waitTime = now - startTime;

    // great big switch get ready
    switch(state)
    {
        case INIT: //-----------------
            if(active)
            {
                state = BtnState::DOWN;
                startTime = now;
                numClicks = 0;
            }
            break;
        case DOWN: //-----------------
            if(!active)
            {
                state = BtnState::UP;
                startTime = now;
            }
            else if (waitTime > Btn::pressMs)
            {
                if(hasOnPressStart)
                    onPressStart();
                state = BtnState::PRESS;
            }
            break;
        case UP: //----------------
            numClicks++;
            state = BtnState::COUNT;
            break;
        case COUNT: //----------------
            if(active)
            {
                state = BtnState::DOWN;
                startTime = now;
            } else if((waitTime >= Btn::clickMs) || (numClicks == maxClicks))
            {
                if(numClicks == 1)
                {
                    if(hasOnClick)
                        onClick();
                } else if (numClicks == 2)
                {
                    if(hasOnDoubleClick)
                        onDoubleClick();
                }
                reset();
            }
            break;
        case PRESS: //-----------------
            if(!active)
                state = BtnState::PRESSEND;
            else
            {
                if((now - lastDuringPressTime) >= pressIntervalMs)
                {
                    if(hasDuringPress)
                        duringPress();
                    lastDuringPressTime = now;
                }
            }
            break;
        case PRESSEND: //-------------------
            if(hasOnPressStop)
                onPressStop();
            reset();
            break;
        default:
            state = BtnState::INIT;
            break; 
    }
}