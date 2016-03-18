#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "Debug.h"
#include "Icon.h"
#include "Passwords.h"

// apps are included here
#include "AlarmApp.h"
#include "AnalogClockApp.h"
#include "BinaryClockApp.h"
#include "BreakoutApp.h"
#include "BriteLiteApp.h"
#include "BritepadApp.h"
#include "BubblesApp.h"
#include "CalculatorApp.h"
#include "CalendarApp.h"
#include "ChromaClockApp.h"
#include "ClockApp.h"
#include "CountdownApp.h"
#include "DotsDisplayApp.h"
#include "FadeDotsApp.h"
#include "GestureApp.h"
#include "HexDotClockApp.h"
#include "InfoApp.h"
#include "KeyApp.h"
#include "KeyboardApp.h"
#include "LifeApp.h"
#include "MinesApp.h"
#include "MuteApp.h"
#include "NumericKeypadApp.h"
#include "PaintApp.h"
#include "PassApp.h"
#include "RebootApp.h"
#include "ScribbleClockApp.h"
#include "SeqApp.h"
#include "SetAlarmApp.h"
#include "SetClockApp.h"
#include "SetTimerApp.h"
#include "SparkleApp.h"
#include "SpiroApp.h"
#include "StarfieldApp.h"
#include "SplashApp.h"
#include "StopwatchApp.h"
#include "TabletApp.h"
#include "ThereminApp.h"
#include "WordClockApp.h"

LauncherApp theLauncherApp;

LauncherApp::LauncherApp() {
  for (int s = 0; s < TOTAL_SCREENS; s++) {
    for (int i = 0; i < buttons_per_screen; i++) {
      setButton(s, i,nullptr);
    }
  }

// left screen contains screensavers and settings
  int p = 0;
  setButton(SCREENSAVERS_SCREEN, p++,  new BubblesApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new DotsDisplayApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new SpiroApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new BriteLiteApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new StarfieldApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new LifeApp);
  setButton(SCREENSAVERS_SCREEN, p++,  new FadeDotsApp);

  p = 0;
  setButton(CLOCKS_SCREEN, p++,  new AnalogClockApp);
  setButton(CLOCKS_SCREEN, p++,  new BinaryClockApp);
  setButton(CLOCKS_SCREEN, p++,  new ClockApp);
  setButton(CLOCKS_SCREEN, p++,  new HexDotClockApp);
  setButton(CLOCKS_SCREEN, p++,  new CountdownApp);
  setButton(CLOCKS_SCREEN, p++,  new WordClockApp);
  setButton(CLOCKS_SCREEN, p++,  new ChromaClockApp);
  setButton(CLOCKS_SCREEN, p++,  new ScribbleClockApp);



  setButton(SETTINGS_SCREEN, 8,   new SetClockApp);
  setButton(SETTINGS_SCREEN, 9,   new MuteApp);
  setButton(SETTINGS_SCREEN, 10,  new SetAlarmApp);

// default screen has quick buttons
  setButton(KEYS_SCREEN, 0,  new KeyApp(volPlusIcon, KEY_MEDIA_VOLUME_INC, screen.bluegreen));
  setButton(KEYS_SCREEN, 4,  new KeyApp(volMinusIcon, KEY_MEDIA_VOLUME_DEC, screen.bluegreen));
  setButton(KEYS_SCREEN, 8,  new KeyApp(muteIcon, KEY_MEDIA_MUTE, screen.blue));

  setButton(KEYS_SCREEN, 5,  new KeyboardApp);
  setButton(KEYS_SCREEN, 6,  new CalculatorApp);
  setButton(KEYS_SCREEN, 7,  new NumericKeypadApp);

  setButton(KEYS_SCREEN, 1,  new KeyApp(rewIcon, KEY_MEDIA_PREV_TRACK, screen.orange));
  setButton(KEYS_SCREEN, 2,  new KeyApp(pauseIcon, KEY_MEDIA_PLAY_PAUSE, screen.orange));
  setButton(KEYS_SCREEN, 3,  new KeyApp(ffIcon, KEY_MEDIA_NEXT_TRACK, screen.orange));

// just for testing
//  setButton(KEYS_SCREEN, 9,  new KeyApp("My Name", "Dean\nBlackketter"));

// i never use this
//  setButton(KEYS_SCREEN, 7,  new KeyApp("Eject", KEY_MEDIA_EJECT));

  setButton(KEYS_SCREEN, 10, new PassApp(PASSWORD_APP_NAME_2, PASSWORD_APP_PASS_2) );
  setButton(KEYS_SCREEN, 11, new PassApp(PASSWORD_APP_NAME_1, PASSWORD_APP_PASS_1) );

  setButton(TIMERS_SCREEN, 0, new SetTimerApp("10 sec", 10));
  setButton(TIMERS_SCREEN, 1, new SetTimerApp("3 min", 3*60));
  setButton(TIMERS_SCREEN, 2, new SetTimerApp("25 min", 25*60));
  setButton(TIMERS_SCREEN, 3, new SetTimerApp("55 min", 55*60));
  setButton(TIMERS_SCREEN, 8, new SetTimerApp());
  setButton(TIMERS_SCREEN, 11, new StopwatchApp);


  p = 0;
  setButton(APPS_SCREEN, p++,  new BreakoutApp);
  setButton(APPS_SCREEN, p++,  new ThereminApp);
  setButton(APPS_SCREEN, p++,  new PaintApp);
  setButton(APPS_SCREEN, p++,  new GestureApp);
  setButton(APPS_SCREEN, p++,  new SparkleApp);
  setButton(APPS_SCREEN, p++,  new TabletApp);
  setButton(APPS_SCREEN, p++,  new CalendarApp);
  setButton(APPS_SCREEN, p++,  new SeqApp);
  setButton(APPS_SCREEN, p++,  new MinesApp);

  setButton(DEBUG_SCREEN, 9,  new InfoApp);
  setButton(DEBUG_SCREEN, 10, new ResetApp);
  setButton(DEBUG_SCREEN, 11, new RebootApp);

  // add mice apps to mice app settings screen
  BritepadApp* iterApp = britepad.getNextApp();
  unsigned int i = 0;
  while (iterApp) {
    if (iterApp->canBeMouse()) {
      setButton(MICE_SCREEN, i++, iterApp);
    }
    iterApp = britepad.getNextApp(iterApp);
  }

  // this gets created, but has no button
  new AlarmApp;

}

void LauncherApp::begin() {
  // do not do the default behaviour, we'll clear the screen ourselves
  //BritepadApp::begin();

  // this should wake up the host, which is great for entering passwords
  // but might have some side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  if (pad.touched(LEFT_PAD)) {
    current_screen = 0;
  } else if (pad.touched(RIGHT_PAD)) {
    current_screen = 2;
  }
  highlighted_button = noButton;

  if (pad.down(TOP_PAD)) {
    sound.swipe(DIRECTION_DOWN);
  }

  screen.pushFill(DIRECTION_DOWN, bgColor());

  drawButtons();
  britepad.disableScreensavers(0);  // reenable screensavers if they were temporarily disabled
}

void LauncherApp::end() {
  if (pad.down(TOP_PAD)) {
    sound.swipe(DIRECTION_UP);
  }

  screen.pushFill(DIRECTION_UP, britepad.getLaunchedApp()->bgColor());
}

void LauncherApp::drawButtons() {

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

  BritepadApp* app = getButton(i);

  bool disabled = !app->getEnabled(screenMode(currentScreen()));
  highlighted = disabled | highlighted;

  color_t color;
  switch (currentScreen()) {
    case MICE_SCREEN:
      color = screen.green;
      break;
    case SCREENSAVERS_SCREEN:
    case CLOCKS_SCREEN:
      color = screen.yellow;
      break;
    default:
      color = app->buttonColor();
      break;
  }

  screen.fillCircle( x, y, radius, highlighted ? screen.mix(color, screen.black) : color);
  const char* name = app->name();
  if (name) {
    screen.setFont(Arial_9_Bold);
    screen.setTextColor(screen.black);
    screen.setCursor( x - screen.measureTextWidth(name) / 2, y - screen.measureTextHeight(name)/2);
    screen.drawText(name);
  } else {
    Icon icon = app->getIcon();
    if (icon.getData()) {
      icon.draw(x - icon.width()/2, y - icon.height()/2, screen.black);
    }
  }
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
  if (screen < TOTAL_SCREENS && i < buttons_per_screen) {
    apps[screen][i] = b;
  } else {
    DEBUG_LN("setButton out of bounds");
  }
};

BritepadApp* LauncherApp::getButton(int i) {
  if ((i >= 0) && (i < buttons_per_screen)) {
    return apps[currentScreen()][i];
  } else {
    return nullptr;
  }
};

AppMode LauncherApp::screenMode(int theScreen) {
  AppMode launchMode;
  switch (current_screen) {
    case MICE_SCREEN:
      launchMode = MOUSE_MODE;
      break;
    case SCREENSAVERS_SCREEN:
    case CLOCKS_SCREEN:
      launchMode = SCREENSAVER_MODE;
      break;
    default:
      launchMode = INTERACTIVE_MODE;
  }
  return launchMode;
}

void LauncherApp::run() {

  lastRun = clock.now();

  int b = buttonHit(pad.x(),pad.y());

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (launchOnRelease) {
    if (pad.up(SCREEN_PAD)) {
      launchApp(launchOnRelease, screenMode(current_screen));
      britepad.disableScreensavers();
      launchOnRelease = nullptr;
    }
  } else if (pad.down(LEFT_PAD)
       || (pad.getGesture() == GESTURE_SWIPE_LEFT)
    ) {
      if (currentScreen() > 0) {
        current_screen--;
        highlighted_button = noButton;
        sound.swipe(DIRECTION_LEFT);
        screen.pushFill(DIRECTION_LEFT, bgColor());
        drawBars();
        drawButtons();
      } else {
        sound.bump();
      }
  } else if (pad.down(RIGHT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_RIGHT)
  ) {
    if (currentScreen() < TOTAL_SCREENS - 1) {
      current_screen++;
      highlighted_button = noButton;
      sound.swipe(DIRECTION_RIGHT);
      screen.pushFill(DIRECTION_RIGHT, bgColor());
      drawBars();
      drawButtons();
    } else {
      sound.bump();
    }
  } else if (pad.getGesture() == GESTURE_SWIPE_UP) {
    exit();
  } else if (!pad.didGesture()) {

     if (pad.up(SCREEN_PAD)) {
      drawButton(highlighted_button, false);

      if (highlighted_button != noButton) {
        BritepadApp* launched = apps[currentScreen()][b];
        if (launched->isPopup()) {
          launched->run();
          if (!launched->isInvisible()) {
            clearScreen();
            drawButtons();
          } else {
            drawButton(b, false);
          }
        } else {
          AppMode whichMode = screenMode(current_screen);
          if (whichMode == INTERACTIVE_MODE) {
            launchApp(launched);
          } else {
            launched->setEnabled(!launched->getEnabled(whichMode), whichMode);
            drawButton(b);
          }
        }
        sound.click();

      }
      highlighted_button = noButton;
    } else if (pad.touched(SCREEN_PAD)) {

      if (b != noButton) {
        if (b != highlighted_button) {
          drawButton(highlighted_button, false);
          drawButton(b, true);
          highlighted_button = b;
        } else {
          if (pad.time() - pad.lastDownTime(SCREEN_PAD) > holdTime) {
            if (getButton(b)) {
              sound.click();
              clearScreen();
              launchOnRelease = getButton(b);
            }
          }
        }
      } else {
        drawButton(highlighted_button, false);
        highlighted_button = noButton;
      }
    }
  } else if (highlighted_button != noButton && pad.touched()) {
      drawButton(highlighted_button, false);
      highlighted_button = noButton;
  }

}

int LauncherApp::currentScreen() {
  // if we haven't run in a while, reset to the middle screen
  if (clock.now() - lastRun > resetScreenTimeout) {
    current_screen = KEYS_SCREEN;
  }

  return current_screen;
}

color_t LauncherApp::bgColor() {
  return screenColor[currentScreen()];
}

const char* LauncherApp::infoBarText() {
  switch (currentScreen()) {
    case SCREENSAVERS_SCREEN:
    case CLOCKS_SCREEN:
    case MICE_SCREEN:
      return "Press and hold to test";
    default:
      return nullptr;
  }
}

const char* LauncherApp::statusBarTitle() {
  return screenNames[currentScreen()];
}
