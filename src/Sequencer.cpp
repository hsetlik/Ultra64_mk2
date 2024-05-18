#include "Sequencer.h"

Ultra64::Ultra64() : display(nullptr),
                     lastDisplayUpdate(0),
                     lastPixelUpdate(0),
                     minLengthColor(127, 127, 200),
                     maxLengthColor(255, 127, 200),
                     offColor(95, 150, 141),
                     onColor(20, 20, 34)
{
}

Ultra64::~Ultra64()
{
    // delete pixels;
    delete display;
}

void Ultra64::init()
{
    // initialize i2c and DAC
    Wire.begin(SDA, SCL, 200000);

    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
    if(!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println("Error! Failed to initialize display!");
    }
    display->clearDisplay();
    display->setRotation(2);
    display->setCursor(2, 0);
    display->setTextColor(SSD1306_WHITE);
    display->println("Initializing. . .");
    display->display();
    delay(1000);
    // pixels
    // set sum default colors
    const uint8_t pHue = 255 / 24;
    const uint8_t hOffset = 10;
    for(uint8_t p = 0; p < 24; p++)
    {
        uint8_t hue = hOffset + (p * pHue);
        CHSV color(hue, 180, 200);
        hsv2rgb_rainbow(color, pixels[p]);
    }
    FastLED.addLeds<NEOPIXEL, PIXEL_PIN>(pixels, 24);
    FastLED.setBrightness(100);
    FastLED.show();
    delay(1000);

    for(uint8_t b = 0; b < 9; b++)
    {
        ButtonID id = (ButtonID)b;
        buttons[b].attachOnClick([this, id](){ this->buttonPressed(id); });
        buttons[b].attachDuringPress([this, id](){this->buttonHeld(id);});
    }

    // now initialize our pixel colors
    const uint8_t dHue = 256 / 12;
    const uint8_t hueOffset = 12;
    for (uint8_t i = 0; i < 12; i++)
    {
        uint8_t hue = hueOffset + (dHue * i);
        CHSV onColor(hue, 180, 200);
        CHSV selectedColor(hue, 220, 250);
        hsv2rgb_rainbow(onColor, noteOnColors[i]);
        hsv2rgb_rainbow(selectedColor, noteSelectedColors[i]);
    }

}

void Ultra64::updateInputs(uint16_t input)
{
    inputState = input;

    // check the encoders
    auto aState = Input::getEncoderState(inputState, 0);
    if(aState.moved)
    {
        encoderTurned(0, aState.up);
    }
    auto bState = Input::getEncoderState(inputState, 1);
    if(bState.moved)
    {
        encoderTurned(1, bState.up);
    }
    auto cState = Input::getEncoderState(inputState, 2);
    if(cState.moved)
    {
        encoderTurned(2, cState.up);
    }
    
    // check the buttons
    for(uint8_t b = 0; b < 9; b++)
    {
        ButtonID id = (ButtonID)b;
        auto state = Input::getButtonState(inputState, id);
        buttons[b].tick(state);
    }
}


//===================================================================================

void Ultra64::encoderTurned(uint8_t enc, bool dir)
{
    switch (enc)
    {
    case 0: // Left encoder: used to select the current step
    {
        shiftSelectedStep(dir);
        break;
    }
    case 1: // Middle encoder: used to set the current pitch or length of the selected note
    {
        if (lengthMode)
            shiftStepLength(dir);
        else
            shiftStepNote(dir);
        break;
    }
    case 2: // Right encoder: used to change the tempo
    {
        shiftTempo(dir);
        break;
    }
    default:
        break;
    }
}

void Ultra64::buttonPressed(ButtonID id)
{
    switch (id)
    {
    case EncA: // toggles note on/off
    {
        auto &s = getSelected();
        s.gate = !s.gate;
        break;
    }
    case EncB: // switches between note mode and length mode
    {
        lengthMode = !lengthMode;
        break;
    }
    case EncC: // play/stop button
    {
        isPlaying = !isPlaying;
        break;
    }
    case C1:
    {
        selectedTrack = 0;
        break;
    }
    case C2:
    {
        selectedTrack = 1;
        break;
    }
    case C3:
    {
        selectedTrack = 2;
        break;
    }
    case C4:
    {
        selectedTrack = 3;
        break;
    }
    case PL: // move selected step left one page
    {
        if (!isPlaying && !quarterMode)
        {
            selectedStep = (selectedStep < 16) ? 48 + selectedStep : selectedStep - 16;
        }
        break;
    }
    case PR: // move right one page
    {
        if (!isPlaying && !quarterMode)
        {
            selectedStep = (selectedStep + 16) % 64;
        }
        break;
    }
    default:
        break;
    }
}

void Ultra64::buttonHeld(ButtonID id)
{
    switch (id)
    {
    case EncA:
    {
        break;
    }
    case EncB:
    {
        break;
    }
    case EncC:
    {
        break;
    }
    case C1:
    {
        seq.clearTrack(0);
        break;
    }
    case C2:
    {
        seq.clearTrack(1);
        break;
    }
    case C3:
    {
        seq.clearTrack(2);
        break;
    }
    case C4:
    {
        seq.clearTrack(3);
        break;
    }
    case PL: // toggles quarter mode
    {
        quarterMode = !quarterMode;
        break;
    }
    case PR:
    {
        break;
    }
    default:
        break;
    }
}

void Ultra64::shiftTempo(bool dir)
{
    if (dir)
        currentTempo = std::min<uint16_t>(currentTempo + 1, TEMPO_MAX);
    else
        currentTempo = std::max<uint16_t>(currentTempo - 1, TEMPO_MIN);
    // update the step length to match the new tempo
    stepLengthMs = 60000.0f / (float)currentTempo;
}

void Ultra64::shiftSelectedStep(bool dir)
{
    const uint8_t max = quarterMode ? 16 : 64;
    if (dir)
    {
        selectedStep = (selectedStep + 1) % max;
    }
    else
    {
        selectedStep = (selectedStep == 0) ? max - 1 : selectedStep - 1;
    }
}

void Ultra64::shiftStepNote(bool dir)
{
    uint8_t &note = getSelected().midiNum;
    if (dir)
    {
        note = std::min<uint8_t>(note + 1, 127);
    }
    else
    {
        note = (note == 0) ? 0 : note - 1;
    }
}

void Ultra64::shiftStepLength(bool dir)
{
    uint8_t &length = getSelected().length;
    if (dir)
    {
        length = std::min<uint8_t>(length + 1, 255);
    }
    else
    {
        length = std::max<uint8_t>(length - 1, LENGTH_MIN);
    }
}
//===================================================================================

float Ultra64::noteLengthMs(uint8_t length)
{
    const float max = stepLengthMs * 0.95f;
    return max * ((float)length / 255.0f);
}

OutputState Ultra64::getOutputs()
{
    OutputState state;
    if (isPlaying)
    {
        // first we need to keep track of time
        msIntoSequence += sampleIntervalMs;
        msIntoStep += sampleIntervalMs;
        if (msIntoStep > stepLengthMs) // time to advance to the next step
        {
            const uint8_t max = (quarterMode) ? 16 : 64;
            currentStep = (currentStep + 1) % max;
            msIntoStep -= stepLengthMs;
            if (currentStep == 0)
            {
                msIntoSequence = msIntoStep;
            }
            // update the gates and control voltages
            for (uint8_t ch = 0; ch < 4; ch++)
            {
                state[ch].gate = seq.tracks[ch][currentStep].gate;
                uint16_t val = (uint16_t)((float)seq.tracks[ch][currentStep].midiNum * HALFSTEP_INCREMENT);
                state[ch].dacValue = val;
            }
        }
        else
        {
            // for any channels that are currently active, check if it's time to switch the gate off
            for (uint8_t ch = 0; ch < 4; ch++)
            {
                auto &step = seq.tracks[ch][currentStep];
                if (step.gate && noteLengthMs(step.length) < msIntoStep)
                    state[ch].gate = false;
            }
        }
    }
    return state;
}

uint64_t Ultra64::getOutputState()
{
    uint64_t state = 0;
     if (isPlaying)
    {
        // first we need to keep track of time
        msIntoSequence += sampleIntervalMs;
        msIntoStep += sampleIntervalMs;
        if (msIntoStep > stepLengthMs) // time to advance to the next step
        {
            const uint8_t max = (quarterMode) ? 16 : 64;
            currentStep = (currentStep + 1) % max;
            msIntoStep -= stepLengthMs;
            if (currentStep == 0)
            {
                msIntoSequence = msIntoStep;
            }
            // update the gates and control voltages
            for (uint8_t ch = 0; ch < 4; ch++)
            {
                Output::setGate(state, ch, seq.tracks[ch][currentStep].gate);
                uint16_t val = (uint16_t)((float)seq.tracks[ch][currentStep].midiNum * HALFSTEP_INCREMENT);
                Output::setDacValue(state, ch, val);
            }
        }
        else
        {
            // for any channels that are currently active, check if it's time to switch the gate off
            for (uint8_t ch = 0; ch < 4; ch++)
            {
                auto &step = seq.tracks[ch][currentStep];
                if (step.gate && noteLengthMs(step.length) < msIntoStep)
                    Output::setGate(state, ch, false);
            }
        }
    }   
    return state;
}
//===================================================================================
void Ultra64::updateDisplay()
{
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    uint8_t yPos = 0;
    for (uint8_t idx = 3; idx >= 0; idx--)
    {
        display->setCursor(2, yPos);
        display->print(log[idx]);
        yPos += 8;
    }
    display->display();
}

void Ultra64::updatePixels()
{
    // step 1: render the sequence pixels
    const uint8_t page = currentStep / 16;
    for (uint8_t p = 0; p < 16; p++)
    {
        uint8_t step = (page * 16) + p;
        auto& stepState = seq.tracks[selectedTrack][step];
        if (!lengthMode)
        {
            if (step == selectedStep)
                pixels[p] = noteSelectedColors[stepState.midiNum % 12];
            else if (stepState.gate)
                pixels[p] = noteOnColors[stepState.midiNum % 12];
            else
                pixels[p] = offColor;
            
        }
        else
        {
            CHSV col = blend(minLengthColor, maxLengthColor, stepState.length);
            if(step == selectedStep)
            {
                col.setHSV(col.hue, 250, 255);
                hsv2rgb_rainbow(col, pixels[p]);
            }
            else if(stepState.gate)
                hsv2rgb_rainbow(col, pixels[p]);
            else
                pixels[p] = offColor;
        }
    }

    // step 2: track indicators
    for(uint8_t t = 0; t < 4; t++)
    {
        if(t == selectedTrack)
            pixels[16 + t] = onColor;
        else
            pixels[16 + t] = offColor;
    }

    // step 3: page indicators
    for(uint8_t p = 0; p < 4; p++)
    {
        if(!quarterMode)
        {
            pixels[20 + p] = (currentStep / 4 == p) ? onColor : offColor;
        }
    }
    // push to the LEDs
    FastLED.show();
}

void Ultra64::tickReadouts()
{
    unsigned long now = millis();
    //update the pixels as needed
    if(now - lastPixelUpdate > PIXEL_INTERVAL)
    {
        updatePixels();
        lastPixelUpdate = now;
    }
    //update the display
    if(now - lastDisplayUpdate > DISPLAY_INTERVAL)
    {
        updateDisplay();
        lastDisplayUpdate = now;
    }

}
//===================================================================================
