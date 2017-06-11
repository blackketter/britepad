#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "widgets/AppButton.h"


keycode_t launchKeys[] = {
KEY_Q, KEY_W, KEY_E, KEY_R,
KEY_A, KEY_S, KEY_D, KEY_F,
KEY_Z, KEY_X, KEY_C, KEY_V
};

screen_t screens[] = {
  { DEBUG_SCREEN,"Debug",nullptr,DEBUG_APP,INTERACTIVE_MODE,Screen::grey},
  { MICE_SCREEN,"Mice","Press and hold to test",MOUSE_APP,MOUSE_MODE,Screen::black},
  { CLOCKS_SCREEN,"Clocks","Press and hold to test",CLOCK_APP,SCREENSAVER_MODE,Screen::darkerred},
  { KEYBOARD_SCREEN,"Keyboard",nullptr,KEYBOARD_APP,KEYBOARD_MODE,Screen::DarkOrange},
  { SCREENSAVERS_SCREEN,"Screensavers","Press and hold to test",SCREENSAVER_APP,SCREENSAVER_MODE,Screen::darkeryellow},
  { SETTINGS_SCREEN, "Settings",nullptr,SETTINGS_APP,INTERACTIVE_MODE,Screen::darkeryellow},
  { HOME_SCREEN,"Home",nullptr,KEY_APP,INTERACTIVE_MODE,Screen::darkergreen},
  { TIMERS_SCREEN,"Timers",nullptr,TIMER_APP,INTERACTIVE_MODE,Screen::darkerblue},
  { APPS_SCREEN,"Apps",nullptr,INTERACTIVE_APP,INTERACTIVE_MODE,Screen::darkergrey},
  { MACROS_SCREEN, "Macros",nullptr, MACROS_APP,INTERACTIVE_MODE,Screen::darkergreen},
  { NO_SCREEN,nullptr,nullptr,INTERACTIVE_APP,INTERACTIVE_MODE,Screen::darkergrey}
};

//  LAUNCHERAPP
color_t LauncherApp::bgColor() {
  return getCurrentScreen()->color;
}

const char* LauncherApp::infoBarText() {
  return getCurrentScreen()->info;
}

const char* LauncherApp::statusBarTitle() {
  return getCurrentScreen()->name;
}

void LauncherApp::setCurrentScreenID(screenid_t n) {
  current_screen = n;
  if (buttons) { delete(buttons); }

  buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),v_buttons,h_buttons,1);

  // do all the specifically positioned apps, then the default (0) position apps
  for (int specific = 1; specific >= 0; specific--) {
    BritepadApp* a = britepad.getNextApp();
    while (a) {
      if (a->isAppType(getCurrentScreen()->type)) {
        int32_t pos = a->getLauncherPosition();
        if ((specific && (pos!=_defaultLauncherPosition)) || (!specific && (pos==_defaultLauncherPosition))) {
          if (pos==_defaultLauncherPosition) { pos = 0; }
          while (buttons->getButton(pos,0)) {
            pos++;
          }
          if (!a->isHidden()) {

            Button* b = nullptr;

            switch (getCurrentScreen()->mode) {
              case KEYBOARD_MODE:
                b = new KeyboardButton(a);
                break;
              case MOUSE_MODE:
                b = new MouseButton(a);
                break;
              case SCREENSAVER_MODE:
                b = new ScreensaverButton(a);
                break;
              case INTERACTIVE_MODE:
                b = a->newAppButton();
                break;
              default:
                break;
            }
            buttons->setButton(b,pos);
          }
        }
      }
      a = britepad.getNextApp(a);
    }
  }
}

screen_t* LauncherApp::getCurrentScreen() {
  int i = 0;
  while (screens[i].id != NO_SCREEN) {
    if (screens[i].id == current_screen) {
      break;
    }
    i++;
  }
  return &screens[i];
}

LauncherApp::LauncherApp() {
  current_screen = HOME_SCREEN;
}

void LauncherApp::begin(AppMode asMode) {

  // go through all the apps and add them to the appropriate screens

  // first, sort the list alphabetically
  britepad.sortApps();

  // this should wake up the host, which is great for entering passwords
  // but might have some side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  if (launch_screen != NO_SCREEN) {
    setCurrentScreenID(launch_screen);
    launch_screen= NO_SCREEN;
  } else if (clock.now() - lastBegin < 2) {
    // launching twice quickly resets to default screen
    setCurrentScreenID(HOME_SCREEN);
  } else if (clock.now() - lastRun > resetScreenTimeout) {
    // if we haven't run in a while, reset to the default screen
    setCurrentScreenID(HOME_SCREEN);
  } else {
    setCurrentScreenID(getCurrentScreenID());
  }

  if (pad.touched(SCREEN_PAD)) {
    waitForRelease = true;
  }

  // adjust the current screen before beginning
  BritepadApp::begin(asMode);

  sound.swipe(DIRECTION_DOWN);

  screen.pushFill(DIRECTION_DOWN, bgColor());

  drawButtons();
  lastBegin = clock.now();
  console.debugln("done LauncherApp::begin");
}

void LauncherApp::drawButtons() {
  console.debugln("drawing buttons");
  buttons->setBounds(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight());
  buttons->draw();
}

void LauncherApp::run() {

  KeyEvent* key = getNextEvent();

  if (pad.released(SCREEN_PAD)) { waitForRelease = false; }

  lastRun = clock.now();
    for (int i = 0; i < buttons_per_screen; i++) {
      if (key && key->pressed(launchKeys[i]) && buttons->getButton(i)) {
        buttons->getButton(i)->setHighlighted(true);
        break;
      }
    }

  if (!launchOnRelease && (key && (key->released(KEY_SPACE) || key->released(KEY_RETURN)))) {
    buttons->setHighlighted(false);
  }

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (launchOnRelease) {
    if (pad.released(SCREEN_PAD) || (key && (key->released(KEY_SPACE) || key->released(KEY_RETURN)))) {
      AppMode runMode = launchOnRelease->canBeSetup() ? SETUP_MODE : getCurrentScreen()->mode;
      launchApp(launchOnRelease, runMode);
      britepad.resetScreensaver(5*60*1000);  // stay running for up to 5 minutes
      launchOnRelease = nullptr;
    }
  } else if (
        pad.pressed(LEFT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_LEFT)
     || (key && key->pressed(KEY_PAGE_UP))
    ) {
      pushScreen(DIRECTION_LEFT);
  } else if (pad.pressed(RIGHT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_RIGHT)
     || (key && key->pressed(KEY_PAGE_DOWN))
  ) {
    pushScreen(DIRECTION_RIGHT);
  } else if (
        (pad.getGesture() == GESTURE_SWIPE_UP)
     || (key && key->pressed(KEY_ESC))
      ) {
    exit();
  } else {
    AppButton* b = (AppButton*)buttons->releasedButton();

    if (!b) {
      for (int i = 0; i < buttons_per_screen; i++) {
        if (key && key->released(launchKeys[i]) && buttons->getButton(i)) {
          b = (AppButton*)buttons->getButton(i);
          b->setHighlighted(false);
          break;
        }
      }
    }

    if (!b && (key && (key->pressed(KEY_SPACE) || key->pressed(KEY_RETURN)))) {
      b = (AppButton*)buttons->getButton(buttons->getSelected(),0);
      if (b) {
        b->setHighlighted(true);
      }
    }
    if (b) {
      BritepadApp* launched = b->getApp();
      if (launched->canBeInvisible()) {
        if (held) {
          held = false;
        } else {
          launched->begin(INVISIBLE_MODE);
          launched->run();
          launched->end();
        }
        b->draw();
      } else {
        AppMode whichMode = getCurrentScreen()->mode;
        if (whichMode == INTERACTIVE_MODE) {
          launchApp(launched);
        } else {
          launched->setEnabled(!launched->getEnabled(whichMode), whichMode);
          b->draw();
        }
      }
      sound.click();
    }
    b = (AppButton*)buttons->heldButton();
    if (b && b->getApp()) {
      BritepadApp* launched = b->getApp();
      if (!launched->canBeSetup() && launched->canBeInvisible()) {
        launched->begin(INVISIBLE_MODE);
        launched->run();
        launched->end();
        held = true;
      } else if (!waitForRelease) {
        sound.click();
        clearScreen();
        launchOnRelease = b->getApp();
      }
    }
  }

  buttonindex_t oldSelection = buttons->getSelected();
  buttonindex_t i = oldSelection;
  if (key && key->pressed(KEY_UP)) {
    do {
      i -= h_buttons;
      if (i < 0) { i += buttons_per_screen; }
    } while (buttons->getButton(i, 0) == nullptr);
    buttons->setSelected(i);
  }

  if (key && key->pressed(KEY_DOWN)) {
    do {
      i += h_buttons;
      if (i >= buttons_per_screen) { i -= buttons_per_screen; }
    } while (buttons->getButton(i, 0) == nullptr);
    buttons->setSelected(i);
  }

  if (key && key->pressed(KEY_LEFT)) {
    do {
      i--;
      if (i < 0) {
        pushScreen(DIRECTION_LEFT);
        i = buttons_per_screen - 1;
      }
    } while (buttons->getButton(i, 0) == nullptr);
    buttons->setSelected(i);
  }

  if (key && (key->pressed(KEY_RIGHT)||key->pressed(KEY_TAB))) {
    do {
      i++;
      if (i >= buttons_per_screen) {
        pushScreen(DIRECTION_RIGHT);
        i = 0;
      }
    } while (buttons->getButton(i, 0) == nullptr);
    buttons->setSelected(i);
  }

  if (oldSelection != i) {
    buttons->draw(i);
    buttons->draw(oldSelection);
  }

}

void LauncherApp::pushScreen(direction_t d) {
    int move = (d == DIRECTION_RIGHT) ? +1 : -1;
    screenid_t newScreen = getCurrentScreenID() + move;
    if (newScreen >= FIRST_SCREEN && newScreen <= LAST_SCREEN) {
      setCurrentScreenID(newScreen);
      sound.swipe(d);
      screen.pushFill(d, bgColor());
      drawBars();
      drawButtons();
    } else {
      sound.bump();
    }
}

void LauncherApp::end() {
  if (buttons) { delete(buttons); buttons = nullptr; }

  if (pad.pressed(TOP_PAD) || audibleExit) {
    sound.swipe(DIRECTION_UP);
    audibleExit = false;
  }

  screen.pushFill(DIRECTION_UP, britepad.getLaunchedApp()->bgColor());
  BritepadApp::end();
}

void LauncherApp::idle(KeyEvent* key) {
  if (key->pressed(KEY_EXIT)) {
    britepad.currentApp()->exit();
  } else if (key->pressed(KEY_RIGHT_FN)) {
    if (!isCurrentApp()) {
      setLaunchScreen(MACROS_SCREEN);
      launch();
    }
  } else if (key->released(KEY_RIGHT_FN)) {
    if (isCurrentApp()) {
      exit();
    }
  }
}

LauncherApp theLauncherApp;
