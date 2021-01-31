#include "BritepadShared.h"

#include "BritepadLauncher.h"
#include "Types.h"
#include "Timer.h"

#include "apps/LauncherApp.h"
#include "apps/ClockApp.h"
#include "apps/SplashApp.h"
#include "apps/AlarmApp.h"

#include "USB/USBHost.h"

BritepadKeyEventQueue britepadKeyEvents;
KeyEventQueue* keyEvents = &britepadKeyEvents;

// for the bell icon
#include "KeyIcons.h"

#define PROXIMITY_DEAD_TIME (1000)

bool BritepadLauncher::setApp(App* newApp, AppMode asMode) {

  if (newApp == App::EXIT_APP) {
    newApp = App::getAppByID(LauncherApp::ID);
    asMode = INTERACTIVE_MODE;
  }

  if (asMode == SCREENSAVER_MODE) {
    screensaverStartedTime = pad.time();
  }

  if (Launcher::setApp(newApp, asMode)) {
    if (currentApp()->usesKeyboard()) {
      keys.update();

      // when a keyboard app launches tell the host that all the keys have been released
      keyEvents->releaseKeys();
    }
    drawBars();
    return true;
  } else {
    return false;
  }
}

void backlightCallback(void* data) {
  uint8_t lastBacklight = screen.getBacklight();

  // if we detect proximity or touch, it's probably casting a shadow and we don't want to update
  millis_t lastTouch = max(pad.lastTouchedTime(ANY_PAD), pad.lastTouchedTime(PROXIMITY_SENSOR));
  if ((pad.time() - lastTouch) > PROXIMITY_DEAD_TIME) {
    // any ambient light greater than 255 is full brightness, 1 is the minimums
    uint8_t light = max(BritepadLauncher::minBrightness,min( pad.getAmbientLight(), 255));

    int delta;
    const int downRate = 2;
    const int upRate = 4;
    // ramp down slow, ramp up fast
    if (lastBacklight < light) {
      if ((light - lastBacklight) > upRate) {
        delta = upRate;
      } else {
        delta = 1;
      }
      lastBacklight += delta;
    } else if (lastBacklight > light) {
      if ((lastBacklight - light) > downRate) {
        delta = downRate;
      } else {
        delta = 1;
      }
      lastBacklight -= delta;
    }

    // backlight not dimmable
    //screen.setBacklight(lastBacklight);
  }
}

void statusBarCallback(void* data) {
    ((BritepadLauncher*)data)->updateStatusBar();
}


void BritepadLauncher::begin() {

  // assumes that the splashapp has been created and added to list
  launchApp(SplashApp::ID, SCREENSAVER_MODE);
  setApp(BritepadApp::getAppByID(SplashApp::ID), SCREENSAVER_MODE);

  Launcher::begin();
  screen.setBacklight(screen.maxbrightness);
  backlightTimer.setMillis(ambientUpdateInterval, backlightCallback, (void*)this, true);
  statusBarUpdateTimer.setMillis(1000, statusBarCallback, (void*)this, true);

	usbHost.begin();
	usbMouse.begin();
}

void BritepadLauncher::idle() {

    watchdogKick();
    usbHost.Task();
    usbMouse.run();
    keys.update();
    sound.idle();
    if (currentApp() && currentApp()->isAppMode(MOUSE_MODE)) {
      mousePad.run();
    };

    Launcher::idle();

};

void BritepadLauncher::loop() {

  pad.update();

  if (pad.touched(ANY_PAD)) {
    resetScreensaver();
  }

  if (!currentBritepadApp()) {
      console.debugln("No currapp!");
      launchApp(LauncherApp::ID);
  }

  if (pad.pressed(TOP_PAD)) {
    currentBritepadApp()->exit();
  } else if (currentBritepadApp()->isAppMode(SCREENSAVER_MODE) && (pad.pressed(SCREEN_PAD) || ((pad.pressed(ANY_PAD) && !currentBritepadApp()->canBeInteractive())))) {
    console.debugln("waking screensaver");
    // waking goes back to the mouse in the case that the user touched the screen (or any touch pad if it's not interactive)
    if (currentBritepadApp()->canBeMouse() && currentBritepadApp()->getEnabled(MOUSE_MODE) && usbActive()) {
      console.debugln("switching current app to MOUSE_MODE");
      currentBritepadApp()->switchAppMode(MOUSE_MODE);
    } else if (usbActive()) {
      console.debugln("launching A_MOUSE_APP");
      launchApp(BritepadApp::A_MOUSE_APP, MOUSE_MODE);
    } else {
      console.debugln("Waking to Launcher");
      launchApp(LauncherApp::ID);
    }

  } else  {
   // check if somebody wants to be screensaver
   if (currentBritepadApp()->isAppMode(SCREENSAVER_MODE) &&
       !currentBritepadApp()->wantsToRun() &&
       wantsToRun()) {

      launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

  // let's check to see if we should run a screensaver
  } else if (pad.time() > disableScreensaversUntil && !currentBritepadApp()->disablesScreensavers()) {

    if ( pad.pressed(PROXIMITY_SENSOR) &&
         currentBritepadApp()->isAppMode(SCREENSAVER_MODE) &&
         !currentBritepadApp()->displaysClock() &&
         BritepadApp::getAppByID(ClockApp::ID))
    {
      launchApp(ClockApp::ID, SCREENSAVER_MODE);
      resetScreensaver(showClockDur);  // disable screensavers for a little while
      sound.click();
      console.debugln("Proximity detected: showing clock");

    } else if (currentBritepadApp()->isAppMode(SCREENSAVER_MODE)
            && getScreensaverSwitchInterval()
            && (pad.time() - screensaverStartedTime) > getScreensaverSwitchInterval()*1000) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);

      // is it time for the screensaver to kick in?
      } else if (!currentBritepadApp()->isAppMode(SCREENSAVER_MODE) && (pad.time() > disableScreensaversUntil)
        && !(currentBritepadApp()->canBeScreensaver() && currentBritepadApp()->isAppMode(MOUSE_MODE))) {
        launchApp(BritepadApp::A_SCREENSAVER_APP, SCREENSAVER_MODE);
      }
    }
  }

  Launcher::loop();
}

void BritepadLauncher::wakeHost() {
  // tap the shift key to wake the computer up
  if (keyEvents->keyIsUp(MODIFIERKEY_LEFT_SHIFT)) {
    Keyboard.press(KEY_RIGHT_ALT);
    Keyboard.release(KEY_RIGHT_ALT);
  }
  // jiggle the mouse to make cursor show up
  Mouse.move(-1, 0);
  Mouse.move(1, 0);

#if 0
  // send system wakeup
  // disabled now because sometimes it puts the system back to sleep
  Keyboard.press(KEY_SYSTEM_WAKE_UP);
  Keyboard.release(KEY_SYSTEM_WAKE_UP);
#endif
}

void BritepadLauncher::updateStatusBar() {
  // updates the time in the status bar
  drawStatusBar(true);
}

void BritepadLauncher::resetClipRect() {
//TODO - Remove need for this cast
  BritepadApp* curr = (BritepadApp*)currentBritepadApp();
  if (curr) {
    coord_t top = curr->displaysStatusBar() ? _statusBarHeight : 0;
    coord_t bottom = curr->displaysInfoBar() ? screen.height()-_statusBarHeight : screen.height();
    curr->setClipRect(0, top, screen.width(), bottom-top);
  }
}

void BritepadLauncher::drawBars(bool update) {
  drawInfoBar(update);
  drawStatusBar(update);
}

BritepadApp* BritepadLauncher::currentBritepadApp() {
  return (BritepadApp*)currentApp();
}

void BritepadLauncher::drawStatusBar(bool update) {
  if (currentBritepadApp()->displaysStatusBar()) {
    screen.setClipRect(0, 0, screen.width(), _statusBarHeight);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentBritepadApp()->statusBarFGColor());

    if (!update) {
      screen.fillScreen(currentBritepadApp()->statusBarBGColor());

      // draw title
      const char* title = currentBritepadApp()->statusBarTitle();
      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(title)) / 2,
                         (_statusBarHeight-screen.measureTextHeight(title)) / 2);
      screen.drawText(title);
    }

    // only include the clock if the app doesn't already draw a clock
    if (!currentBritepadApp()->displaysClock() && clock.hasBeenSet()) {
      // draw title
      screen.setFont(&Arial_8_Bold);
      color_t textColor = screen.mix(currentBritepadApp()->statusBarFGColor(), currentBritepadApp()->statusBarBGColor());
      screen.setTextColor(textColor, currentBritepadApp()->statusBarBGColor());
      char shortTime[20];
      clock.shortTime(shortTime);
      char shortTimeSpaced[100];
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( (screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 2),
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);

      AlarmApp* alarm = (AlarmApp*)BritepadApp::getAppByID(AlarmApp::ID);
      if (alarm && alarm->getAlarmEnabled()) {
        Icon(bellIcon).draw( screen.clipRight() - screen.measureTextWidth(shortTimeSpaced) - 10, 4, textColor) ;
      }

      clock.shortDate(shortTime);
      sprintf(shortTimeSpaced,"  %s ", shortTime);
      screen.setCursor( screen.clipLeft() + 2,
                        (_statusBarHeight-screen.measureTextHeight(shortTimeSpaced)) / 2);
      screen.drawText(shortTimeSpaced);
    }

  }
  resetClipRect();
}

void BritepadLauncher::drawInfoBar(bool update) {
  if (currentBritepadApp()->displaysInfoBar()) {
    coord_t top = screen.height()-_statusBarHeight;
    screen.setClipRect(0, top, screen.width(), screen.height()-top);

    screen.setFont(&Arial_8_Bold);
    screen.setTextColor(currentBritepadApp()->infoBarFGColor());

    const char* text = currentBritepadApp()->infoBarText();
    if (!update && text) {
      screen.fillScreen(currentBritepadApp()->infoBarBGColor());

      screen.setCursor( (screen.clipWidth() - screen.measureTextWidth(text)) / 2,
                         top + (_statusBarHeight-screen.measureTextHeight(text)) / 2);
      screen.drawText(text);
    }

  }
   resetClipRect();
}

class QuitCommand : public Command {
  const char* getName() { return "quit"; }
  const char* getHelp() { return "exit the current app"; }
  void execute(Console* c, uint8_t paramCount, char** params) {
    launcher.exit();
  }
};
QuitCommand theQuitCommand;

