// PlatformIO needs these for some reason
#include "SPI.h"
#include "EEPROM.h"
#include "Wire.h"

/* britepad sketch*/
#include "BritepadShared.h"

Screen screen = Screen();
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Britepad britepad = Britepad();
Preferences prefs = Preferences();
MousePad mouse = MousePad();
Console console = Console();
KeyboardMatrix keyboardMatrix = KeyboardMatrix();

//US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};
Timezone localTimezone(usPDT,usPST);
Clock clock = Clock();

void setup() {
  // this is the magic trick for printf to support float
  asm(".global _printf_float");

  // this is the magic trick for scanf to support float
  // not needed right now
  // asm(".global _scanf_float");

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(1000);
  console.begin();

  console.debugln("britepad starting...");

  delay(1000);
  console.executeCommandLine("i2c");

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

  console.debugln("starting clock");
  clock.setZone(&localTimezone);
  clock.updateTime();

  console.debugln("starting keyboard matrix");
  keyboardMatrix.begin();

  Wire.setClock(400000);

  console.debugln("starting app framework");
  britepad.begin();
}

void loop() {
  britepad.idle();
}
