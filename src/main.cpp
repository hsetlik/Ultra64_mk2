#include "Sequencer.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
//
Ultra64 seq;

RotaryEncoder encA(ENCA_L, ENCA_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encB(ENCB_L, ENCB_R, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encC(ENCC_L, ENCC_R, RotaryEncoder::LatchMode::TWO03);

Adafruit_MCP23X17 expander;
Adafruit_MCP4728 dac;

// Volatile vars for the input ISR
volatile long encAPos = 0;
volatile long encBPos = 0;
volatile long encCPos = 0;
volatile uint16_t inputState;
volatile bool newInputsReady = false;

// vars for the output ISR 

volatile uint64_t prevOutputs;
volatile uint64_t currentOutputs;
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
  Input::setButtonState(is, EncA, expander.digitalRead(ENCA_B));
  Input::setButtonState(is, EncB, expander.digitalRead(ENCB_B));
  Input::setButtonState(is, EncC, expander.digitalRead(ENCC_B));
  Input::setButtonState(is, C1, expander.digitalRead(CH1));
  Input::setButtonState(is, C2, expander.digitalRead(CH2));
  Input::setButtonState(is, C3, expander.digitalRead(CH3));
  Input::setButtonState(is, C4, expander.digitalRead(CH4));
  Input::setButtonState(is, EncC, expander.digitalRead(ENCC_B));
  Input::setButtonState(is, EncC, expander.digitalRead(ENCC_B));

  inputState = is;
  newInputsReady = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

// void ARDUINO_ISR_ATTR outputISR()
// {
//   portENTER_CRITICAL_ISR(&timerMux);
//   // grip the new values
//   if(currentOutputs != prevOutputs)
//   {
//     for(uint8_t ch = 0; ch < 4; ch++)
//     {
//       if(Output::getDacValue(prevOutputs, ch) != Output::getDacValue(currentOutputs, ch))
//       {

//       }
//     }

//   }
//   portEXIT_CRITICAL_ISR(&timerMux);
// }
//===============================================================

void setup()
{
  // start serial communication
  Serial.begin(115200);
  // initialize sequencer
  seq.init();
  seq.pushMessage("Seq. Initialized!");
  // set pin modes
  pinMode(EXP_CS, OUTPUT);
  digitalWrite(EXP_CS, HIGH);
  // set up SPI and initialize the MCP23S17 expander
  SPI.begin(SCK, MISO, MOSI, EXP_CS);
  if (!expander.begin_SPI(EXP_CS, &SPI, HW_ADDR))
  {
    Serial.println("Error! Failed to initialize IO expander!");
  }
  //set up DAC
  if(!dac.begin())
  {
    Serial.println("Failed to initialize DAC!");
  }

  //set up hardware timers and interrupts
  inTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(inTimer, &inputISR, true);
  timerAlarmWrite(inTimer, 1000000 / INPUT_POLLING_HZ, true);
  timerAlarmEnable(inTimer);
  Serial.println("Attached input interrupt");


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
