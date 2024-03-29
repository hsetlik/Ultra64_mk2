#pragma once
#include <Adafruit_MCP23X17.h>
#include <functional>

#define HOLD_TICKS 1000

class MCPButton
{
private:
    const uint8_t pin;
    Adafruit_MCP23X17* const mcp;
    // keep track of the state of the button for debouncing and fancy types of presses and stuff
    bool prevState;
    unsigned long ticksSinceChange;
    
    // function pointers for our press/hold events
    bool onPressSet;
    bool onHoldSet;
    bool holdLatch;
    std::function<void()> onPress;
    std::function<void()> onHold;
public:
    MCPButton(uint8_t pin, Adafruit_MCP23X17* exp);
    void tick();

    void setOnPress(std::function<void()> func)
    {
        onPressSet = true;
        onPress = func;
    }
    void setOnHold(std::function<void()> func)
    {
        onHoldSet = true;
        onHold = func;
    }
};