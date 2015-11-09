/* britepad sketch*/

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
