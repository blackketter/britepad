#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "Debug.h"

// apps are included here
#include "AnalogClockApp.h"
#include "BinaryClockApp.h"
#include "BreakoutApp.h"
#include "BriteLiteApp.h"
#include "BritepadApp.h"
#include "BubblesApp.h"
#include "ClockApp.h"
#include "DotsDisplayApp.h"
#include "FadeDotsApp.h"
#include "GestureApp.h"
#include "InfoApp.h"
#include "KeyApp.h"
#include "KeyboardApp.h"
#include "LifeApp.h"
#include "MuteApp.h"
#include "PaintApp.h"
#include "PassApp.h"
#include "RebootApp.h"
#include "SetClockApp.h"
#include "SetTimerApp.h"
#include "SplashApp.h"
#include "StopwatchApp.h"
#include "ThereminApp.h"

LauncherApp theLauncherApp;

LauncherApp::LauncherApp(void) {
  for (int s = 0; s < TOTAL_SCREENS; s++) {
    for (int i = 0; i < buttons_per_screen; i++) {
      setButton(s, i,nil);
    }
  }

// left screen contains screensavers and settings
  setButton(SCREENSAVERS_SCREEN, 0,  new BubblesApp);
  setButton(SCREENSAVERS_SCREEN, 1,  new SplashApp);
  setButton(SCREENSAVERS_SCREEN, 2,  new DotsDisplayApp);
  setButton(SCREENSAVERS_SCREEN, 3,  new ClockApp);
  setButton(SCREENSAVERS_SCREEN, 4,  new BriteLiteApp);
  setButton(SCREENSAVERS_SCREEN, 5,  new BinaryClockApp);
  setButton(SCREENSAVERS_SCREEN, 6,  new LifeApp);
  setButton(SCREENSAVERS_SCREEN, 7,  new FadeDotsApp);
  setButton(SCREENSAVERS_SCREEN, 8,  new AnalogClockApp);

  setButton(SETTINGS_SCREEN, 8,  new SetClockApp);
  setButton(SETTINGS_SCREEN, 9,  new MuteApp);

// middle screen has quick buttons
  setButton(KEYS_SCREEN, 0,  new KeyApp("Vol+", KEY_MEDIA_VOLUME_INC, screen.bluegreen));
  setButton(KEYS_SCREEN, 4,  new KeyApp("Vol-", KEY_MEDIA_VOLUME_DEC, screen.bluegreen));
  setButton(KEYS_SCREEN, 8,  new KeyApp("Mute", KEY_MEDIA_MUTE, screen.blue));

  setButton(KEYS_SCREEN, 5,  new KeyboardApp);

  setButton(KEYS_SCREEN, 1,  new KeyApp("<<", KEY_MEDIA_PREV_TRACK, screen.orange));
  setButton(KEYS_SCREEN, 2,  new KeyApp("||", KEY_MEDIA_PLAY_PAUSE, screen.orange));
  setButton(KEYS_SCREEN, 3,  new KeyApp(">>", KEY_MEDIA_NEXT_TRACK, screen.orange));

// just for testing
//  setButton(KEYS_SCREEN, 9,  new KeyApp("My Name", "Dean\nBlackketter"));

// i never use this
//  setButton(KEYS_SCREEN, 7,  new KeyApp("Eject", KEY_MEDIA_EJECT));

  setButton(KEYS_SCREEN, 10, new PassApp("iCloud", "Bu77cracks!\n") );
  setButton(KEYS_SCREEN, 11, new PassApp("Pass", "ch@db1ldr3n\n") );

// right screen has useful apps
  setButton(TIMERS_SCREEN, 0, new SetTimerApp("10 sec", 10));
  setButton(TIMERS_SCREEN, 1, new SetTimerApp("3 min", 3*60));
  setButton(TIMERS_SCREEN, 2, new SetTimerApp("25 min", 25*60));
  setButton(TIMERS_SCREEN, 3, new SetTimerApp("55 min", 55*60));


  setButton(TIMERS_SCREEN, 11, new StopwatchApp);

  setButton(APPS_SCREEN, 0,  new BreakoutApp);
  setButton(APPS_SCREEN, 1,  new ThereminApp);
  setButton(APPS_SCREEN, 2,  new PaintApp);
  setButton(APPS_SCREEN, 3,  new GestureApp);
  setButton(APPS_SCREEN, 10, new InfoApp);
//  setButton(APPS_SCREEN, 11,  new RebootApp);  // todo: doesn't work yet
}

void LauncherApp::begin(AppMode asMode) {

  // this should wake up the host, which is great for entering passwords
  // but might have some ugly side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  if (pad.touched(LEFT_PAD)) {
    current_screen = 0;
  } else if (pad.touched(RIGHT_PAD)) {
    current_screen = 2;
  }

  screen.pushFill(DIRECTION_DOWN, bgColor());
  drawButtons();
}

void LauncherApp::end(BritepadApp* nextApp) {
  screen.pushFill(DIRECTION_UP, nextApp->bgColor());
}

void LauncherApp::drawButtons(void) {

  for (int i = 0; i < buttons_per_screen; i++) {
    if (getButton(i)) {
        drawButton(i);
    }
  }
}

void LauncherApp::drawButton(int i, bool highlighted) {
  if (i == noButton || i >= buttons_per_screen) {
    return;
  }
  // todo: factor out these into function that finds button coordinates
  const int vsize = screen.clipHeight() / v_buttons;
  const int hsize = screen.clipWidth() / h_buttons;

  int x = hsize/2 + hsize*(i%h_buttons) + screen.clipLeft();
  int y = vsize/2 + vsize*(i/h_buttons) + screen.clipTop();

  const int radius = min(hsize,vsize) / 2 - 2;

  screen.fillCircle( x, y, radius,
    highlighted ? screen.mix(apps[currentScreen()][i]->buttonColor(), screen.black) : apps[currentScreen()][i]->buttonColor());
  const char* name = apps[currentScreen()][i]->name();
  screen.setFont(Arial_9_Bold);
  screen.setTextColor(screen.black);
  screen.setCursor( x - screen.measureTextH(name) / 2, y - screen.measureTextV(name)/2);
  screen.drawText(name);
}

int LauncherApp::buttonHit(int x, int y) {

  int h = (x - screen.clipLeft()) / (screen.clipWidth() / h_buttons);
  int v = (y - screen.clipTop()) / (screen.clipHeight() / v_buttons);

  int i = v*h_buttons+h;
  if (getButton(i)) {
    return i;
  } else {
    return noButton;
  }
}

void LauncherApp::setButton(int screen, int i, BritepadApp* b)
{
  apps[screen][i] = b;
};

BritepadApp* LauncherApp::getButton(int i) {
  if ((i >= 0) && (i < buttons_per_screen)) {
    return apps[currentScreen()][i];
  } else {
    return nil;
  }
};

BritepadApp* LauncherApp::run(void) {

  lastRun = clock.now();

  BritepadApp* exit = nil;  // by default, don't exit

  int b = buttonHit(pad.x(),pad.y());

  if (pad.touched(SCREEN_PAD)) {

    if (b != noButton) {
      if (b != highlighted_button) {
        drawButton(highlighted_button, false);
        drawButton(b, true);
        highlighted_button = b;
      }
    } else {
      drawButton(highlighted_button, false);
      highlighted_button = noButton;
    }
  } else {

    if (pad.up(SCREEN_PAD)) {
      drawButton(highlighted_button, false);
//      DEBUG_PARAM_LN("released on button", highlighted_button);

      if (highlighted_button != noButton) {
        BritepadApp* launched = apps[currentScreen()][b];
        if (launched->isPopup()) {
          exit = launched->run();
          if (!launched->isInvisible()) {
            clearScreen();
            drawButtons();
          } else {
            drawButton(b, false);
          }
        } else {
          // todo handle screensavers
          if (launched->canBeScreensaver()) {

            // toggle the enabledness of the screensaver, and launch it if we're enabling it
            launched->setEnabled(!launched->getEnabled());
            if (launched->getEnabled()) {
              exit = launched;
            } else {
              drawButton(b, false);
              exit = STAY_IN_APP;
            }
          } else {
            exit = launched;
          }
        }
        sound.click();

      } else {
 // disable exiting by tapping on empty space to enable swiping to edge and back
 //      exit = DEFAULT_APP;
      }
      highlighted_button = noButton;
    }
  }

  if (pad.down(LEFT_PAD)) {
    if (currentScreen() > 0) {
      current_screen--;
      sound.swipe(DIRECTION_LEFT);
      screen.pushFill(DIRECTION_LEFT, bgColor());
      drawBars();
      drawButtons();
    } else {
      sound.bump();
    }
  }

  if (pad.down(RIGHT_PAD)) {
    if (currentScreen() < TOTAL_SCREENS - 1) {
      current_screen++;
      sound.swipe(DIRECTION_RIGHT);
      screen.pushFill(DIRECTION_RIGHT, bgColor());
      drawBars();
      drawButtons();
    } else {
      sound.bump();
    }
  }

  return exit;
}

int LauncherApp::currentScreen(void) {
  // if we haven't run in a while, reset to the middle screen
  if (clock.now() - lastRun > resetScreenTimeout) {
    current_screen = KEYS_SCREEN;
  }

  return current_screen;
}

color_t LauncherApp::bgColor(void) {
  return screenColor[currentScreen()];
}

const char* LauncherApp::infoBarText(void) {
  return currentScreen() == SCREENSAVERS_SCREEN ? "Press and hold to test" : nullptr;
}

const char* LauncherApp::statusBarTitle(void) {
  return screenNames[currentScreen()];
}
