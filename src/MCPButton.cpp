#include "MCPButton.h"

MCPButton::MCPButton(uint8_t p, Adafruit_MCP23X17* exp) :
pin(p),
mcp(exp),
prevState(true),
ticksSinceChange(0),
onPressSet(false),
onHoldSet(false),
holdLatch(false)
{

}

void MCPButton::tick()
{
    bool currentState = mcp->digitalRead(pin);
    if(currentState != prevState)
    {
        holdLatch = false;
        // check for releasing a short press
        if(onPressSet && currentState && ticksSinceChange > 4)
        {
            onPress();
        }
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