#include "Sequencer.h"

//
Ultra64 seq;

RotaryEncoder encA(ENCA_L, ENCA_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encB(ENCB_L, ENCB_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encC(ENCC_L, ENCC_R, RotaryEncoder::LatchMode::TWO03);

Adafruit_MCP23X17 exp;
Adafruit_MCP4728 dac;

// Volatile vars for the input ISR
volatile long encAPos = 0;
volatile long encBPos = 0;
volatile long encCPos = 0;
volatile uint16_t inputState;
volatile bool newInputsReady = false;

// vars for the output ISR 

volatile uint64_t prevOutputs;
volatile bool needsNewOutputs = false;

//hardware timers for our two ISRs
hw_timer_t* inTimer = NULL;
hw_timer_t* outTimer = NULL;

// critical section
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//our ISR functions
void ARDUINO_ISR_ATTR inputISR()
{
  portENTER_CRITICAL_ISR(&timerMux);

  uint16_t is = 0;
  // encoders
  encA.tick();
  auto aPos = encA.getPosition();
  if(aPos != encAPos)
  {
    Input::addEncoderState(is, 0, aPos < encAPos);
  }

  encB.tick();
  auto bPos = encB.getPosition();
  if(bPos != encBPos)
  {
    Input::addEncoderState(is, 1, bPos < encBPos);
  }

  encC.tick();
  auto cPos = encC.getPosition();
  if(cPos != encCPos)
  {
    Input::addEncoderState(is, 2, cPos < encCPos);
  }


  // buttons
  Input::setButtonState(is, EncA, exp.digitalRead(ENCA_B));
  Input::setButtonState(is, EncB, exp.digitalRead(ENCB_B));
  Input::setButtonState(is, EncC, exp.digitalRead(ENCC_B));
  Input::setButtonState(is, C1, exp.digitalRead(CH1));
  Input::setButtonState(is, C2, exp.digitalRead(CH2));
  Input::setButtonState(is, C3, exp.digitalRead(CH3));
  Input::setButtonState(is, C4, exp.digitalRead(CH4));
  Input::setButtonState(is, EncC, exp.digitalRead(ENCC_B));
  Input::setButtonState(is, EncC, exp.digitalRead(ENCC_B));

  inputState = is;
  newInputsReady = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void ARDUINO_ISR_ATTR outputISR()
{
  portENTER_CRITICAL_ISR(&timerMux);
  // grip the new values
  uint64_t currentOutputs = seq.getOutputState();
  if(currentOutputs != prevOutputs)
  {

  }
  portEXIT_CRITICAL_ISR(&timerMux);
}
//===============================================================

void setup()
{
  // start serial communication
  Serial.begin(115200);
  // initialize sequencer
  seq.init();
  // set pin modes
  pinMode(EXP_CS, OUTPUT);
  digitalWrite(EXP_CS, HIGH);
  // set up SPI and initialize the MCP23S17 expander
  SPI.begin(SCK, MISO, MOSI, EXP_CS);
  if (!exp.begin_SPI(EXP_CS, &SPI, HW_ADDR))
  {
    Serial.println("Error! Failed to initialize IO expander!");
  }
  //set up DAC
  if(!dac.begin())
  {
    Serial.println("Failed to initialize DAC!");
  }

}

void loop()
{
  // put your main code here, to run repeatedly:
  seq.tickReadouts();
  if(newInputsReady)
  {
    seq.updateInputs(inputState);
    newInputsReady = false;
  }
}
