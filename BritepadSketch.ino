/* britepad sketch*/

#include "BritepadShared.h"

#define DEBUG_ON 1
#if 0
#include "ILI9341_t3.h"
#include "Debug.h"
#include "FastLED.h"
#include "TimeLib.h"
#include "EEPROM.h"
#include "Audio.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "Wire.h"
#include "Adafruit_FT6206.h"
#include <SparkFun_APDS9960.h>
#endif

Screen screen = Screen();
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Clock clock = Clock();
Britepad britepad = Britepad();
Preferences prefs = Preferences();
MousePad mouse = MousePad();

void setup() {

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(500);

  DEBUG_INIT();

  DEBUG_LN("britepad starting...");

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
