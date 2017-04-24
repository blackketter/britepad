#ifndef _Hardware_
#define _Hardware_

#include "Arduino.h"

typedef int pinNumber;
bool usbActive();
uint32_t FreeMem();

#ifdef TEENSYDUINO
// touchpad pins
const pinNumber B_TOUCH_PIN = A2;  // DIGITAL 16
const pinNumber T_TOUCH_PIN = A8;  // DIGITAL 22
const pinNumber L_TOUCH_PIN = A1;  // DIGITAL 15
const pinNumber R_TOUCH_PIN = A3;  // DIGITAL 17

// touch thresholds
const int L_HIGH_THRESHOLD = 950;
const int L_LOW_THRESHOLD = 900;

const int R_HIGH_THRESHOLD = 1300;
const int R_LOW_THRESHOLD = 1100;

const int B_HIGH_THRESHOLD = 1100;
const int B_LOW_THRESHOLD = 1000;

const int T_HIGH_THRESHOLD = 1200;
const int T_LOW_THRESHOLD = 1100;

// prox/light sensor interrupt pin
const pinNumber APDS9960_INT_PIN = 20;  // Needs to be an interrupt pin

// capacitive touch panel interrupt pin
const pinNumber CTP_INT_PIN = 14;

// audio amplifier shutdown (active low)
const pinNumber AUDIO_SHUTDOWN_PIN = 23;

const pinNumber WIRE_SDA_PIN = 18;
const pinNumber WIRE_SCL_PIN = 19;

// screen
const pinNumber TFT_CS_PIN = 10;
const pinNumber TFT_DC_PIN = 9;
const pinNumber BACKLIGHT_PIN = 21;

#elif defined(ESP8266)

const pinNumber TFT_DC_PIN = 2;
const pinNumber TFT_CS_PIN  = 4;
const pinNumber BACKLIGHT_PIN  = 0;

#endif

#endif
