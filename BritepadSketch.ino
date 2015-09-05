/* britepad sketch*/

// these have to be here to satisfy the Arduino build system
#include <ILI9341_t3.h>
#include <SPI.h>       // this is needed for display
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <usb_keyboard.h>
#include <FastLED.h>
#include <Time.h>
#include <Audio.h>
#include <SD.h>
#include <SerialFlash.h>

#include "BritepadShared.h"
#include "Timer.h"

#define DEBUG_ON 1
#include "Debug.h"

#define TFT_CS 10
#define TFT_DC 9


Screen screen = Screen(TFT_CS, TFT_DC);
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Britepad britepad = Britepad();

void setup(void) {
  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(500);

  // set clock to a recent time
  setTime(4,20,0,1, 1, 2015);

  DEBUG_INIT();

  DEBUG_LN("britepad starting...");

  screen.begin();
  screen.setRotation(3);  // Britepad Mark-0 is oriented this way
  DEBUG_LN("screen started");

  pad.begin();
  DEBUG_LN("touchpad started");

  sound.begin();

  DEBUG_LN("starting app framework");
  britepad.begin();
}

void loop() {
  britepad.idle();
}
