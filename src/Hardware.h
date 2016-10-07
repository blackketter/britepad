#ifndef _Hardware_
#define _Hardware_

#include "Arduino.h"

typedef int pinNumber;
bool usbActive();

#ifdef TEENSYDUINO

// touchpad pins
const pinNumber B_TOUCH_PIN = A2;
const pinNumber T_TOUCH_PIN = A8;
const pinNumber L_TOUCH_PIN = A1;
const pinNumber R_TOUCH_PIN = A3;

// touch thresholds
const pinNumber L_HIGH_THRESHOLD = 950;
const pinNumber L_LOW_THRESHOLD = 900;

const pinNumber R_HIGH_THRESHOLD = 1300;
const pinNumber R_LOW_THRESHOLD = 1100;

const pinNumber B_HIGH_THRESHOLD = 1100;
const pinNumber B_LOW_THRESHOLD = 1000;

const pinNumber T_HIGH_THRESHOLD = 1150;
const pinNumber T_LOW_THRESHOLD = 1050;

// prox/light sensor pinNumbererrupt
const pinNumber APDS9960_INT = 20;  // Needs to be an interrupt pin

// screen
const pinNumber TFT_CS = 10;
const pinNumber TFT_DC = 9;
const pinNumber BACKLIGHT_PIN = 21;

#elif defined(ESP8266)

const pinNumber TFT_DC = 2;
const pinNumber TFT_CS  = 4;
const pinNumber BACKLIGHT_PIN  = 0;

#endif

#endif
