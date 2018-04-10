// PlatformIO needs these for some reason
#include "SPI.h"
#include "EEPROM.h"
#include "Wire.h"

/* britepad sketch*/
#include "BritepadShared.h"
#include "ErgodoxLayout.h"

Screen screen = Screen();
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();
Britepad britepad = Britepad();
EEPROMDictionary prefs = EEPROMDictionary();

MousePad mousePad = MousePad();
Console console = Console();
ErgodoxKeyMatrix keys;

//US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, dowSunday, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, dowSunday, Nov, 2, -480};
Timezone localTimezone(usPDT,usPST);
Clock clock = Clock();

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

  console.begin();
  console.setPort(&Serial);
  console.debugln("britepad starting...");

  console.executeCommandLine("i2c");

  console.debugln("starting prefs...");
  prefs.load();

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
  keys.begin();

  Wire.setClock(WIRE_SPEED);

  watchdogKick();

  console.debugln("starting app framework");
  britepad.begin();

  Dictionary foo = Dictionary();
  console.debugf("empty foo used: %d\n", foo.used());
  console.debugf("foo xxx size: %d\n", foo.size("xxx"));

  String v;
  String n = "xxx";
  console.debugf("foo missing size: %d\n", foo.get(n,v));

//  foo.reset();

  if (!foo.set("def", 5,(const uint8_t*)"4567")) console.debugln("def add failed");
  console.debugf("def added, foo used: %d\n", foo.used());

  if (!foo.set("abc", 4,(const uint8_t*)"123")) console.debugln("ghi add failed");
  console.debugf("abc added, foo used: %d\n", foo.used());

  if (!foo.set("ghi", 5,(const uint8_t*)"qwer")) console.debugln("ghi add failed");
  console.debugf("ghi added, used: %d\n", foo.used());

  uint8_t temp[5];

  console.debugf("foo get abc: %d (%s)\n", foo.get("abc", 4, temp), temp);
  console.debugf("foo get def: %d (%s)\n", foo.get("def", 5, temp), temp);
  console.debugf("foo get ghi: %d (%s)\n", foo.get("ghi", 5, temp), temp);

  n = "abc";
  console.debugf("foo %s size: %d\n", n.c_str(), foo.get(n,v));
  console.debugf("foo abc value: %s\n",v.c_str());

  n = "def";

  console.debugf("foo def size: %d\n", foo.get(n,v));
  console.debugf("foo def value: %s\n",v.c_str());

  console.debugln("deleting def");
  foo.set("def");

  n = "def";

  console.debugf("foo def size: %d\n", foo.get(n,v));
  console.debugf("foo def value: %s\n",v.c_str());

  n = "ghi";
  console.debugf("foo ghi size: %d\n", foo.get(n,v));
  console.debugf("foo ghi value: %s\n",v.c_str());

      const uint8_t* data = foo.data();
      size_t len = foo.used();
      for (size_t i = 0; i < len; i++) {
        console.debugf("foo[%04d] = %02x '%c'\n", i, data[i], data[i]);
      }
}

void loop() {
  britepad.loop();
  watchdogKick();
}
