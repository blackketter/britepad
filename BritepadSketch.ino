/* britepad sketch*/

// these have to be here to satisfy the Arduino build system
#include <ILI9341_t3.h>
#include <SPI.h>       // this is needed for display
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <SparkFun_APDS9960.h>
#include <usb_keyboard.h>
#include <FastLED.h>
#include <Time.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>
#include <EEPROM.h>
#include <font_ArialBold.h>

#include "BritepadShared.h"

#define DEBUG_ON 1
#include "Debug.h"

#define TFT_CS 10
#define TFT_DC 9


Screen screen = Screen(TFT_CS, TFT_DC);
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Clock clock = Clock();
Britepad britepad = Britepad();
Preferences prefs = Preferences();

void setup(void) {

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(500);

  DEBUG_INIT();

  DEBUG_LN("britepad starting...");
  DEBUG_PARAM_LN("Teensy3Clock.get()", year(Teensy3Clock.get()));

  DEBUG_LN("starting screen");
  screen.begin();
  screen.setRotation(3);  // Britepad Mark-0 is oriented this way

  DEBUG_LN("starting touchpad");
  pad.begin();

  DEBUG_LN("starting sound");
  sound.begin();

  DEBUG_LN("starting app framework");
  britepad.begin();
}

void loop() {
  britepad.idle();
}
