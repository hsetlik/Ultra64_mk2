#include "Sequencer.h"

Ultra64::Ultra64() : 
pixels(nullptr),
encA(nullptr),
encB(nullptr),
encC(nullptr),
encAPos(0),
encBPos(0),
encCPos(0),
encAButton(nullptr),
encBButton(nullptr),
encCButton(nullptr),
c1(nullptr),
c2(nullptr),
c3(nullptr),
c4(nullptr),
pLeft(nullptr),
pRight(nullptr)
{

}

Ultra64::~Ultra64()
{
    delete pixels;
    delete encA;
    delete encB;
    delete encC;

    delete encAButton;
    delete encBButton;
    delete encCButton;
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete pLeft;
    delete pRight;
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

    // buttons
    encAButton = new MCPButton(ENCA_B, &exp);
    encAButton->setOnPress([this](){this->buttonPressed(EncA);});
    encAButton->setOnHold([this](){this->buttonHeld(EncA);});

    encBButton = new MCPButton(ENCB_B, &exp);
    encBButton->setOnPress([this](){this->buttonPressed(EncB);});
    encBButton->setOnHold([this](){this->buttonHeld(EncB);});

    encCButton = new MCPButton(ENCC_B, &exp);
    encCButton->setOnPress([this](){this->buttonPressed(EncC);});
    encCButton->setOnHold([this](){this->buttonHeld(EncC);});

    c1 = new MCPButton(CH1, &exp);
    c1->setOnPress([this](){this->buttonPressed(C1);});
    c1->setOnHold([this](){this->buttonHeld(C1);});

    c2 = new MCPButton(CH2, &exp);
    c2->setOnPress([this](){this->buttonPressed(C2);});
    c2->setOnHold([this](){this->buttonHeld(C2);});

    c3 = new MCPButton(CH3, &exp);
    c3->setOnPress([this](){this->buttonPressed(C3);});
    c3->setOnHold([this](){this->buttonHeld(C3);});

    c4 = new MCPButton(CH4, &exp);
    c4->setOnPress([this](){this->buttonPressed(C4);});
    c4->setOnHold([this](){this->buttonHeld(C4);});

    pLeft = new MCPButton(P_LEFT, &exp);
    pLeft->setOnPress([this](){this->buttonPressed(PL);});
    pLeft->setOnHold([this](){this->buttonHeld(PL);});

    pRight = new MCPButton(P_RIGHT, &exp);
    pRight->setOnPress([this](){this->buttonPressed(PR);});
    pRight->setOnHold([this](){this->buttonHeld(PR);});

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
    //check buttons
    for(auto* b : buttons)
    {
        b->tick();
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

void Ultra64::buttonPressed(ButtonID id)
{
    switch(id)
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
            break;
        }
        case C2:
        {
            break;
        }
        case C3:
        {
            break;
        }
        case C4:
        {
            break;
        }
        case PL:
        {
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

void Ultra64::buttonHeld(ButtonID id)
{
    switch(id)
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
            break;
        }
        case C2:
        {
            break;
        }
        case C3:
        {
            break;
        }
        case C4:
        {
            break;
        }
        case PL:
        {
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
//===================================================================================