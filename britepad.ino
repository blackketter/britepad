/* britepad */

// these have to be here to satisfy the Arduino build system
#include <ILI9341_t3.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>
#include <usb_keyboard.h>
#include <FastLED.h>
#include <Time.h>

#include "Britepad.h"

#define DEBUG_ON 1
#include "Debug.h"

// hardware libraries (screen, touchscreen/pads)
#include "Screen.h"
#include "TouchPad.h"

// apps are included here
#include "BPApp.h"
#include "BubblesApp.h"
#include "MouseApp.h"
#include "LauncherApp.h"
#include "SplashApp.h"
#include "PassApp.h"
#include "ICPassApp.h"
#include "KeyApp.h"
#include "SetClockApp.h"
#include "ClockApp.h"
#include "DotsDisplayApp.h"

#define SCREENSAVER_DELAY (5000)

#define TFT_CS 10
#define TFT_DC 9


Screen screen = Screen(TFT_CS, TFT_DC);
TouchPad pad = TouchPad(screen.width(), screen.height());

BPApp* mouseApp;
BPApp* screensaverApp;
LauncherApp* launcherApp;
BPApp* splashApp;

BPApp* currApp;

void setApp(BPApp* newApp) {
  if (newApp == 0) {
    DEBUG_LN("Set currApp to NIL!");
  }

  if (newApp == currApp) {
    return;
  }

  if (currApp)
    currApp->end();

  currApp = newApp;

  if (currApp)
    currApp->begin();
}

void setup(void) {
  // delay at startup, not sure why it's needed to get the cpu unstuck
  delay(1000);

  // set clock to a recent time
  setTime(4,20,0,1, 1, 2015);

  DEBUG_INIT();

  DEBUG_LN(F("britepad starting"));

  screen.begin();
  screen.setRotation(3);  // britepad mark 0 is oriented this way

  DEBUG_LN("drawing inited");

  pad.begin();
  DEBUG_LN("Capacitive touchscreen started");

// TODO : move this to MouseApp initializer
  Mouse.begin();

  launcherApp = new LauncherApp;
  screensaverApp = new BubblesApp;
  mouseApp = new MouseApp;
  splashApp = new SplashApp;

// left screen
  launcherApp->setButton(0, 0,  screensaverApp);
  launcherApp->setButton(0, 1,  mouseApp);
  launcherApp->setButton(0, 2,  splashApp);
  launcherApp->setButton(0, 8,  new SetClockApp);
  launcherApp->setButton(0, 7,  new ClockApp);

// middle screen
  launcherApp->setButton(1, 0,  new KeyApp("Vol+", KEY_MEDIA_VOLUME_INC));
  launcherApp->setButton(1, 4,  new KeyApp("Vol-", KEY_MEDIA_VOLUME_DEC));
  launcherApp->setButton(1, 7,  new KeyApp("Mute", KEY_MEDIA_MUTE));

  launcherApp->setButton(1, 1,  new KeyApp("<<", KEY_MEDIA_PREV_TRACK));
  launcherApp->setButton(1, 2,  new KeyApp("||", KEY_MEDIA_PLAY_PAUSE));
  launcherApp->setButton(1, 3,  new KeyApp(">>", KEY_MEDIA_NEXT_TRACK));

  launcherApp->setButton(1, 8,  new KeyApp("Eject", KEY_MEDIA_EJECT));

  launcherApp->setButton(1, 9,  new KeyApp("My\nFull\nName", "Dean\nBlackketter"));
  launcherApp->setButton(1, 10, new ICPassApp);
  launcherApp->setButton(1, 11, new PassApp);

// right screen
  launcherApp->setButton(2, 6,  new DotsDisplayApp);
  setApp(splashApp);
}

void loop() {

  static long screensaver_started = 0;

  pad.update();

  if (pad.touched(ANY_PAD)) {
    // touching resets screensaver
    if (screensaver_started) {
      screensaver_started = 0;
      setApp(mouseApp);
    }
  } else {
    if (pad.time() - pad.lastUpTime(ANY_PAD) > SCREENSAVER_DELAY) {
     if (screensaver_started == 0) {
        screensaver_started = pad.time();
      }
    }
  }

  if (pad.down(TOP_PAD)) {
    if (currApp != launcherApp) {
      setApp(launcherApp);
    }  else {
      setApp(mouseApp);
    }
  } else if (pad.down(ANY_PAD)) {
//    setApp(mouseApp);
  } else if (screensaver_started && !currApp->isScreensaver() ) {
    setApp(screensaverApp);
  }

  if (currApp) {
    BPApp* newApp = currApp->run();
    if (newApp == DEFAULT_APP) {
      newApp = mouseApp;
    } else if (newApp == BACK_APP) {
      newApp = launcherApp;
    }

    if (newApp) {
      setApp(newApp);
    }
  } else {
    DEBUG_LN("currApp nil!");
  }

}
