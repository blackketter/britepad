#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "AppButton.h"

//  LAUNCHERAPP
color_t LauncherApp::bgColor() {
  return screenColor[getCurrentScreen()];
}

const char* LauncherApp::infoBarText() {
  return infoText[getCurrentScreen()];
}

const char* LauncherApp::statusBarTitle() {
  return screenNames[getCurrentScreen()];
}

void LauncherApp::setCurrentScreen(int n) {
  current_screen = n;
  buttons->setMap(n);
}

void LauncherApp::begin(AppMode asMode) {
  if (buttons) { delete(buttons); }
  buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),v_buttons,h_buttons,TOTAL_SCREENS);

  // go through all the apps and add them to the appropriate screens

  // first, sort the list alphabetically
  britepad.sortApps();

  // do all the specifically positioned apps, then the default (0) position apps
  for (int specific = 1; specific >= 0; specific--) {
    BritepadApp* a = britepad.getNextApp();

    while (a) {
      for (int curScreen = 0; curScreen < TOTAL_SCREENS; curScreen++) {
        if (a->isAppType(screenTypes[curScreen])) {
          int32_t pos = a->getLauncherPosition();
          if ((specific && (pos!=defaultLauncherPosition)) || (!specific && (pos==defaultLauncherPosition))) {
            if (pos==defaultLauncherPosition) { pos = 0; }
            while (buttons->getButton(pos,curScreen)) {
              pos++;
            }
            if (!a->isHidden()) {
              //console.debugf("Adding %s to screen %s at position %d\n", a->name(), screenNames[curScreen], pos);

              Button* b = nullptr;

              switch (screenMode[curScreen]) {
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
              buttons->setButton(b,pos, curScreen);
            }
          }
        }
      }
      a = britepad.getNextApp(a);
    }
  }
  buttons->setMap(getCurrentScreen());

  // this should wake up the host, which is great for entering passwords
  // but might have some side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  if (pad.touched(LEFT_PAD)) {
    setCurrentScreen(0);
  } else if (pad.touched(RIGHT_PAD)) {
    setCurrentScreen(2);
  } else if (clock.now() - lastBegin < 2) {
    // launching twice quickly resets to default screen
    setCurrentScreen();
  } else if (clock.now() - lastRun > resetScreenTimeout) {
    // if we haven't run in a while, reset to the default screen
    setCurrentScreen();
  }

  if (pad.touched(SCREEN_PAD)) {
    waitForRelease = true;
  }

  // adjust the current screen before beginning
  BritepadApp::begin(asMode);

  if (pad.down(TOP_PAD)) {
    sound.swipe(DIRECTION_DOWN);
  }

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

void LauncherApp::idle() {
  if (!isCurrentApp()) {
    if (keyMatrix.keyPressed((keycode_t)KEY_HOME)) {
      britepad.resetScreensaver();
      keyMatrix.clearKeyChange((keycode_t)KEY_HOME);
      launch();
    }
  } else if (keyMatrix.keyPressed((keycode_t)KEY_ESC)
          || keyMatrix.keyPressed((keycode_t)KEY_HOME)) {
      keyMatrix.clearKeyChange((keycode_t)KEY_HOME);
      exit();
    }
}

void LauncherApp::run() {

  if (pad.up(SCREEN_PAD)) { waitForRelease = false; }

  lastRun = clock.now();

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (launchOnRelease) {
    if (pad.up(SCREEN_PAD)) {
      launchApp(launchOnRelease, screenMode[getCurrentScreen()]);
      britepad.resetScreensaver(5*60*1000);  // stay running for up to 5 minutes
      launchOnRelease = nullptr;
    }
  } else if (
        pad.down(LEFT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_LEFT)
     || keyMatrix.keyPressed((keycode_t)KEY_PAGE_UP)
    ) {
      if (getCurrentScreen() > 0) {
        setCurrentScreen(getCurrentScreen() - 1);
        sound.swipe(DIRECTION_LEFT);
        screen.pushFill(DIRECTION_LEFT, bgColor());
        drawBars();
        drawButtons();
      } else {
        sound.bump();
      }
  } else if (pad.down(RIGHT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_RIGHT)
     || keyMatrix.keyPressed((keycode_t)KEY_PAGE_DOWN)
  ) {
    if (getCurrentScreen() < TOTAL_SCREENS - 1) {
      setCurrentScreen(getCurrentScreen()+1);
      sound.swipe(DIRECTION_RIGHT);
      screen.pushFill(DIRECTION_RIGHT, bgColor());
      drawBars();
      drawButtons();
    } else {
      sound.bump();
    }
  } else if (
        (pad.getGesture() == GESTURE_SWIPE_UP)
     || keyMatrix.keyPressed((keycode_t)KEY_ESC)
     || keyMatrix.keyPressed((keycode_t)KEY_HOME)
      ) {
    exit();
  } else if (!pad.didGesture()) {
    AppButton* b = (AppButton*)(buttons->up());
    if (b) {
      BritepadApp* launched = ((AppButton*)b)->getApp();
      if (launched->isInvisible()) {
        if (held) {
          held = false;
        } else {
          launched->begin(INTERACTIVE_MODE);
          launched->run();
          launched->end();
        }
        b->draw();
      } else {
        AppMode whichMode = screenMode[getCurrentScreen()];
        if (whichMode == INTERACTIVE_MODE) {
          launchApp(launched);
        } else {
          launched->setEnabled(!launched->getEnabled(whichMode), whichMode);
          b->draw();
        }
      }
      sound.click();
    }
    b = (AppButton*)buttons->hold();
    if (b && b->getApp()) {
      BritepadApp* launched = ((AppButton*)b)->getApp();
      if (b->getApp()->isInvisible()) {
        launched->begin(INTERACTIVE_MODE);
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
}

void LauncherApp::end() {
  if (buttons) { delete(buttons); buttons = nullptr; }

  if (pad.down(TOP_PAD)) {
    sound.swipe(DIRECTION_UP);
  }

  screen.pushFill(DIRECTION_UP, britepad.getLaunchedApp()->bgColor());
  BritepadApp::end();
}
