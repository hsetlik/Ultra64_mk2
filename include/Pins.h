#pragma once
#include <Arduino.h>

// SPI
#define SCK 14
#define MOSI 13
#define MISO 12
#define EXP_CS 21
#define EXP_RST 9
#define HW_ADDR 0b110

// I2C
#define SDA 18
#define SCL 17
#define LDAC 8

// Pixels
#define PIXEL_PIN 39

// Rotary Encoders
#define ENCA_L 4
#define ENCA_R 5

#define ENCB_L 41
#define ENCB_R 40

#define ENCC_L 6
#define ENCC_R 39

//==========================================================
// MCP23S17 I/O Expander pins

#define CH1 2
#define CH2 3
#define CH3 4
#define CH4 5
#define P_LEFT 6
#define P_RIGHT 7

#define GATE1 8
#define GATE2 9
#define GATE3 10
#define GATE4 11
#define ENCA_B 12
#define ENCB_B 13
#define ENCC_B 14

// port A bit indeces

#define CH1_BIT 2
#define CH2_BIT 3
#define CH3_BIT 4
#define CH4_BIT 5
#define P_LEFT_BIT 6
#define P_RIGHT_BIT 7

// port B bit indeces

#define GATE1_BIT 0
#define GATE2_BIT 1
#define GATE3_BIT 2
#define GATE4_BIT 3
#define ENCC_BIT 4
#define ENCB_BIT 5
#define ENCA_BIT 6

