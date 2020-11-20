// PlatformIO needs these for some reason
#include "SPI.h"
#include "EEPROM.h"
#include "Wire.h"

/* britepad sketch*/
#include "BritepadShared.h"
#include "ErgodoxLayout.h"

#ifdef TEENSYDEBUG
#include "TeensyDebug.h"
#pragma GCC optimize ("O0")
#endif

Screen screen = Screen();
TouchPad pad = TouchPad();
Sound sound = Sound();
Britepad britepad = Britepad();
EEPROMDictionary prefs = EEPROMDictionary();

MousePad mousePad = MousePad();
Console console = Console();

#include "USB/USBKeyMatrix.h"
#include "apps/KeyboardViewerApp.h"

#if defined(BB1)
BB1KeyMatrix keys;
#elif defined(BB100)
BB100KeyMatrix keys;
#endif

KeyboardViewerApp theKeyboardViewerApp(&keys);

USBKeyMatrix usbKeys;
KeyboardViewerApp theUSBKeyboardViewerApp(&usbKeys);


Clock clock;

//US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};
Timezone localTimezone(usPDT,usPST);

void setup() {

  // setup hardware
  watchdogKick();

  // this is the magic trick for printf to support float
  asm(".global _printf_float");

  // this is the magic trick for scanf to support float
  // not needed right now
  // asm(".global _scanf_float");

  pinMode(AUDIO_SHUTDOWN_PIN, OUTPUT);
  digitalWrite(AUDIO_SHUTDOWN_PIN, HIGH ); // turn on the amplifier

  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(500);

#ifdef TEENSYDEBUG
  debug.begin(SerialUSB1);
#endif

  console.begin();
  console.setPort(&Serial);
  console.debugln("britepad starting...");

  console.debugln("starting prefs...");
  prefs.load();

  console.debugln("starting screen");
  screen.begin();
  screen.setRotation(1);  // So far, all displays are in this rotation
  screen.fillScreen(screen.black);

  console.debugln("starting touchpad");
  pad.begin(screen.width(), screen.height());

  console.debugln("starting sound");
  sound.begin();

  console.debugln("starting clock");
  clock.setZone(&localTimezone);
  clock.updateTime();

  console.debugln("starting keyboard matrix");
  keys.begin(&keyEvents);

  Wire.setClock(WIRE_SPEED);

  watchdogKick();

  console.debugln("starting app framework");
  britepad.begin();
}

void loop() {
  britepad.loop();
}
