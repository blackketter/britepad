#ifndef _Hardware_
#define _Hardware_

#include "Arduino.h"

#ifndef WATCHDOG_TIME_MS
#define WATCHDOG_TIME_MS (2000)
#endif

// system hardware utilities
bool usbActive();
uint32_t FreeMem();
void watchdogKick();
const char* resetType();

typedef int pinNumber;

#ifdef TEENSYDUINO
// touchpad pins
const pinNumber B_TOUCH_PIN = A2;  // DIGITAL 16
const pinNumber T_TOUCH_PIN = A8;  // DIGITAL 22
const pinNumber L_TOUCH_PIN = A1;  // DIGITAL 15
const pinNumber R_TOUCH_PIN = A3;  // DIGITAL 17

#ifdef __MK66FX1M0__
const pinNumber AMBIENT_ANALOG_PIN = A20;
#else
const pinNumber AMBIENT_ANALOG_PIN = A11;
#endif

// touch thresholds
const int L_HIGH_THRESHOLD = 950;
const int L_LOW_THRESHOLD = 900;

const int R_HIGH_THRESHOLD = 1300;
const int R_LOW_THRESHOLD = 1100;

const int B_HIGH_THRESHOLD = 1100;
const int B_LOW_THRESHOLD = 1000;

const int T_HIGH_THRESHOLD = 2000;
const int T_LOW_THRESHOLD = 1500;

// prox/light sensor interrupt pin
const pinNumber APDS9960_INT_PIN = 20;  // Needs to be an interrupt pin

// audio amplifier shutdown (active low)
const pinNumber AUDIO_SHUTDOWN_PIN = 23;

// i2c bus config
const pinNumber WIRE_SDA_PIN = 18;
const pinNumber WIRE_SCL_PIN = 19;
const uint32_t  WIRE_SPEED = 400000;

#if defined(LCD_ERTFTM0784)

const pinNumber TFT_CS_PIN = 10;
const pinNumber TFT_RESET_PIN = 14;
const pinNumber TFT_MOSI_PIN = 11;
const pinNumber TFT_SCLK_PIN = 13;
const pinNumber TFT_MISO_PIN = 12;

const pinNumber TFT_2828CS_PIN = 36;
const pinNumber TFT_2828RESET_PIN = 38;
const pinNumber TFT_2828SDI_PIN = 4;
const pinNumber TFT_2828SCK_PIN = 37;

const pinNumber BACKLIGHT_PIN = 9;

// capacitive touch panel interrupt and reset pins
const pinNumber CTP_INT_PIN = 39;
const pinNumber CTP_RESET_PIN = 3;

#else
// screen
const pinNumber TFT_CS_PIN = 10;
const pinNumber TFT_DC_PIN = 9;
const pinNumber BACKLIGHT_PIN = 21;
#endif

#elif defined(ESP32)

const pinNumber TFT_DC_PIN = 33;
const pinNumber TFT_CS_PIN  = 15;
const pinNumber BACKLIGHT_PIN  = 22;
const pinNumber TFT_RST_PIN = 16;

const pinNumber AUDIO_SHUTDOWN_PIN = 0;

const uint32_t  WIRE_SPEED = 400000;

#endif

#endif
