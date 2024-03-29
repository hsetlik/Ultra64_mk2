#include "Sequencer.h"

Ultra64::Ultra64() : 
pixels(nullptr),
encA(nullptr),
encB(nullptr),
encC(nullptr),
encAPos(0),
encBPos(0),
encCPos(0)
{

}

Ultra64::~Ultra64()
{
    delete pixels;
    delete encA;
    delete encB;
    delete encC;
}

void Ultra64::init()
{
    // set pin modes
    pinMode(EXP_CS, OUTPUT);
    digitalWrite(EXP_CS, HIGH);
    //set up SPI and initialize the MCP23S17 expander
    SPI.begin(SCK, MOSI, MISO, EXP_CS);
    if(!exp.begin_SPI(EXP_CS, &SPI, HW_ADDR))
    {
        Serial.println("Error! Failed to initialize IO expander!");
    }
    // pixels
    pixels = new Adafruit_NeoPixel(24, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
    pixels->begin();

    //encoders
    encA = new RotaryEncoder(ENCA_L, ENCA_R, RotaryEncoder::LatchMode::TWO03); 
    encB = new RotaryEncoder(ENCB_L, ENCB_R, RotaryEncoder::LatchMode::TWO03); 
    encC = new RotaryEncoder(ENCC_L, ENCC_R, RotaryEncoder::LatchMode::TWO03); 
}


void Ultra64::pollInputs()
{
    // check encoders
    long aPos = encA->getPosition();
    if(aPos != encAPos)
    {
        encoderTurned(0, aPos < encAPos);
        encAPos = aPos;
    }

    long bPos = encB->getPosition();
    if(bPos != encBPos)
    {
        encoderTurned(1, bPos < encBPos);
        encBPos = bPos;
    }

    long cPos = encC->getPosition();
    if(cPos != encCPos)
    {
        encoderTurned(2, cPos < encCPos);
        encCPos = cPos;
    }
}

//===================================================================================
void Ultra64::updateOutputs()
{

}


void Ultra64::encoderTurned(uint8_t enc, bool dir)
{
    switch(enc)
    {
        case 0: // Left encoder: used to select the current step
        {
            break;
        }
        case 1: // Middle encoder: used to set the current pitch or length of the selected note
        {
            break;
        }
        case 2: // Right encoder: used to change the tempo
        {
            break;
        }
        default:
            break;
    }
}
//===================================================================================