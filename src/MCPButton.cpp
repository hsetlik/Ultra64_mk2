#include "MCPButton.h"

MCPButton::MCPButton(uint8_t p) :
pin(p),
prevState(true),
ticksSinceChange(0),
onPressSet(false),
onHoldSet(false),
holdLatch(false)
{

}

void MCPButton::tick()
{
    bool currentState = false;
    if(currentState != prevState)
    {
        // check for releasing a short press
        if(onPressSet && currentState && ticksSinceChange > 4 && !holdLatch)
        {
            onPress();
        }
        holdLatch = false;
        ticksSinceChange = 0;
        prevState = currentState;
    }
    else
    {
        ++ticksSinceChange;
        if(onHoldSet && !holdLatch && ticksSinceChange >= HOLD_TICKS)
        {
            onHold();
            holdLatch = true;
        }
    }
}

void MCPButton::updateState(bool currentState)
{

}