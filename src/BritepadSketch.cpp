// PlatformIO needs these for some reason
#include "SPI.h"
#include "EEPROM.h"

/* britepad sketch*/
#include "BritepadShared.h"
#include "Debug.h"

#define DEBUG_ON 1

Screen screen = Screen();
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
TeensyRTCClock clock = TeensyRTCClock();
Britepad britepad = Britepad();
Preferences prefs = Preferences();
MousePad mouse = MousePad();

void setup() {
  // this is the magic trick for printf to support float
  asm(".global _printf_float");

  // this is the magic trick for scanf to support float
  // not needed right now
  // asm(".global _scanf_float");

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(5000);

  DEBUG_INIT();

  DEBUG_LN("britepad starting...");

  DEBUG_LN("starting prefs...");
  prefs.begin();

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
