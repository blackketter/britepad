// PlatformIO needs these for some reason
#include "SPI.h"
#include "EEPROM.h"

/* britepad sketch*/
#include "BritepadShared.h"


Screen screen = Screen();
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Clock clock = Clock();
Britepad britepad = Britepad();
Preferences prefs = Preferences();
MousePad mouse = MousePad();
Console console = Console();

void setup() {
  // this is the magic trick for printf to support float
  asm(".global _printf_float");

  // this is the magic trick for scanf to support float
  // not needed right now
  // asm(".global _scanf_float");

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(2000);

  console.begin();

  console.debugln("britepad starting...");

  console.debugln("starting prefs...");
  prefs.begin();

  console.debugln("starting screen");
  screen.begin();
  screen.setRotation(3);  // Britepad Mark-0 is oriented this way
  screen.fillScreen(screen.black);

  console.debugln("starting touchpad");
  pad.begin();

  console.debugln("starting sound");
  sound.begin();

  console.debugln("starting app framework");
  britepad.begin();
}

void loop() {
  britepad.idle();
}
