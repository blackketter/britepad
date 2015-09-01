/* britepad */

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

#include "Britepad.h"

#define DEBUG_ON 1
#include "Debug.h"

// britepad hardware libraries (screen, touchscreen/pads)
#include "Screen.h"
#include "TouchPad.h"

// apps are included here
#include "BritepadApp.h"
#include "ScreensaverApp.h"
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
#include "TimerApp.h"
#include "SetTimerApp.h"
#include "MuteApp.h"
#include "ThereminApp.h"
#include "Stopwatch.h"

#define SCREENSAVER_DELAY (10000)

#define TFT_CS 10
#define TFT_DC 9


Screen screen = Screen(TFT_CS, TFT_DC);
TouchPad pad = TouchPad(screen.width(), screen.height());
Sound sound = Sound();

LauncherApp* launcherApp;
MouseApp*    mouseApp;
BritepadApp* splashApp;
BritepadApp* currApp;

void setApp(BritepadApp* newApp) {
  if (newApp == 0) {
    DEBUG_LN("Set currApp to NIL!");
  }

  if (newApp == currApp) {
    return;
  }

  if (currApp)
    currApp->end(newApp);

  currApp = newApp;

  if (currApp) {
    currApp->updateStatusBar(true);
    currApp->begin();
  }
}

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

  launcherApp = new LauncherApp;
  mouseApp = new MouseApp;
  splashApp = new SplashApp;
  timerApp = new TimerApp;

  ScreensaverApp* bubblesApp = new BubblesApp;

// left screen contains screensavers and settings
  launcherApp->setButton(0, 0,  bubblesApp);
  launcherApp->setButton(0, 1,  splashApp);
  launcherApp->setButton(0, 2,  new DotsDisplayApp);
  launcherApp->setButton(0, 3,  new ClockApp);

// isn't needed explicitly
//  launcherApp->setButton(0, 4,  timerApp);

  launcherApp->setButton(0, 8,  new SetClockApp);
  launcherApp->setButton(0, 9,  new MuteApp);

// middle screen has quick buttons
  launcherApp->setButton(1, 0,  new KeyApp("Vol+", KEY_MEDIA_VOLUME_INC, screen.bluegreen));
  launcherApp->setButton(1, 4,  new KeyApp("Vol-", KEY_MEDIA_VOLUME_DEC, screen.bluegreen));
  launcherApp->setButton(1, 8,  new KeyApp("Mute", KEY_MEDIA_MUTE, screen.bluegreen));

  launcherApp->setButton(1, 1,  new KeyApp("<<", KEY_MEDIA_PREV_TRACK, screen.orange));
  launcherApp->setButton(1, 2,  new KeyApp("||", KEY_MEDIA_PLAY_PAUSE, screen.orange));
  launcherApp->setButton(1, 3,  new KeyApp(">>", KEY_MEDIA_NEXT_TRACK, screen.orange));

// i never use this
//  launcherApp->setButton(1, 7,  new KeyApp("Eject", KEY_MEDIA_EJECT));

  launcherApp->setButton(1, 10, new ICPassApp);
  launcherApp->setButton(1, 11, new PassApp);

// right screen has useful apps
  launcherApp->setButton(2, 0, new SetTimerApp("10 sec", 10));
  launcherApp->setButton(2, 1, new SetTimerApp("3 min", 3*60));
  launcherApp->setButton(2, 2, new SetTimerApp("25 min", 25*60));
  launcherApp->setButton(2, 3, new SetTimerApp("55 min", 55*60));
  launcherApp->setButton(2, 4, new SetTimerApp("10:05", 10*60+5));
  launcherApp->setButton(2, 5, new StopwatchApp);

  launcherApp->setButton(2, 8,  new ThereminApp);

  launcherApp->setButton(2, 9,  new KeyApp("My Name", "Dean\nBlackketter"));

  launcherApp->setButton(2, 11,  mouseApp);

// show the splash screen
  setApp(splashApp);


// set the current screensaver
  currentScreensaver = bubblesApp;
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
     if (screensaver_started == 0 && !currApp->disablesScreensavers()) {
        screensaver_started = pad.time();
      }
    }
  }

  if (pad.down(TOP_PAD)) {
    if (currApp != launcherApp) {
      setApp(launcherApp);
      pad.update();  // consume that down event
    }
  } else if (!pad.down(ANY_PAD) && screensaver_started) {
      if (timerApp->timerActive()) {
        setApp(timerApp);
      } else {
        if (!currApp->isScreensaver() ) {
          setApp(currentScreensaver);
        }
      }
  }

  if (currApp) {
    BritepadApp* newApp = currApp->run();
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
