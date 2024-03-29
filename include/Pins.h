#pragma once
#include <Arduino.h>

// SPI
#define SCK 14
#define MOSI 13
#define MISO 12
#define EXP_CS 2
#define EXP_RST 7

// I2C
#define SDA 17
#define SCL 18
#define DAC_RDY 8
#define LDAC 9

// Pixels
#define PIXEL_PIN 21

// Rotary Encoders
#define ENCA_L 4
#define ENCA_R 5

#define ENCB_L 41
#define ENCB_R 40

#define ENCC_L 6
#define ENCC_R 39

//==========================================================
// MCP23S17 I/O Expander pins

#define ENCA_B 8
#define ENCB_B 9
#define ENCC_B 10

#define CH1 13
#define CH2 14
#define CH3 15
#define CH4 0

#define GATE1 2
#define GATE2 3
#define GATE3 4
#define GATE4 5

#define P_LEFT 6
#define P_RIGHT 7
