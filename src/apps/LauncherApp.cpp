#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"

LauncherApp::LauncherApp() {
  for (int s = 0; s < TOTAL_SCREENS; s++) {
    for (int i = 0; i < buttons_per_screen; i++) {
      setButton(s, i,nullptr);
    }
  }


  // go through all the apps and add them to the appropriate screens

  // first, sort the list alphabetically
  britepad.sortApps();
  // first, do all the specifically positioned apps, then the default (0) position apps
  for (int specific = 1; specific >= 0; specific--) {
    BritepadApp* a = britepad.getNextApp();

    while (a) {
      for (int screen = 0; screen < TOTAL_SCREENS; screen++) {
        if (a->isAppType(screenTypes[screen])) {
          int32_t pos = a->getLauncherPosition();
          if ((specific && (pos!=defaultLauncherPosition)) || (!specific && (pos==defaultLauncherPosition))) {
            if (pos==defaultLauncherPosition) { pos = 0; }
            while (getApp(screen, pos)) {
              pos++;
            }
            if (!a->isHidden()) {
              console.debugf("Adding %s to screen %s at position %d\n", a->name(), screenNames[screen], pos);
              setButton(screen, pos, a);
            }
          }
        }
      }
      a = britepad.getNextApp(a);
    }
  }
}

void LauncherApp::begin(AppMode asMode) {

  // this should wake up the host, which is great for entering passwords
  // but might have some side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  if (pad.touched(LEFT_PAD)) {
    setCurrentScreen(0);
  } else if (pad.touched(RIGHT_PAD)) {
    setCurrentScreen(2);
  } else {
    checkTimeout();
  }

  if (pad.touched(SCREEN_PAD)) {
    waitForRelease = true;
  }

  // adjust the current screen before beginning
  BritepadApp::begin(asMode);

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
  BritepadApp::end();
}

void LauncherApp::drawButtons() {

  for (int i = 0; i < buttons_per_screen; i++) {
    if (getApp(i)) {
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

  BritepadApp* app = getApp(i);

  bool disabled = !app->getEnabled(screenMode[getCurrentScreen()]);
  highlighted = disabled | highlighted;

  color_t color;
  switch (getCurrentScreen()) {
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
    screen.setTextColor(screen.black);
    char* line2 = strchr(name, '\n');
    if (line2) {
      line2++;
      screen.setFont(Arial_8_Bold);
      char line[strlen(name)+1];
      strcpy(line,name);
      *(strchr(line,'\n')) = 0;
      screen.setCursor( x - screen.measureTextWidth(line) / 2, y - screen.measureTextHeight(line)-2);
      screen.drawText(line);
      screen.setCursor( x - screen.measureTextWidth(line2) / 2, y+2);
      screen.drawText(line2);
    } else {
      screen.setFont(Arial_9_Bold);
      screen.setCursor( x - screen.measureTextWidth(name) / 2, y - screen.measureTextHeight(name)/2);
      screen.drawText(name);
    }
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
  if (getApp(i)) {
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
    console.debugln("setButton out of bounds");
  }
};

BritepadApp* LauncherApp::getApp(int i) {
  if ((i >= 0) && (i < buttons_per_screen)) {
    return apps[getCurrentScreen()][i];
  } else {
    return nullptr;
  }
};


void LauncherApp::run() {

  if (pad.up(SCREEN_PAD)) { waitForRelease = false; }

  lastRun = clock.now();

  int b = buttonHit(pad.x(),pad.y());

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (launchOnRelease) {
    if (pad.up(SCREEN_PAD)) {
      launchApp(launchOnRelease, screenMode[getCurrentScreen()]);
      britepad.disableScreensavers();
      launchOnRelease = nullptr;
    }
  } else if (pad.down(LEFT_PAD)
       || (pad.getGesture() == GESTURE_SWIPE_LEFT)
    ) {
      if (getCurrentScreen() > 0) {
        setCurrentScreen(getCurrentScreen() - 1);
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
    if (getCurrentScreen() < TOTAL_SCREENS - 1) {
      setCurrentScreen(getCurrentScreen()+1);
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
        BritepadApp* launched = apps[getCurrentScreen()][b];
        if (launched->isPopup()) {
          launched->run();
          if (!launched->isInvisible()) {
            clearScreen();
            drawButtons();
          } else {
            drawButton(b, false);
          }
        } else {
          AppMode whichMode = screenMode[getCurrentScreen()];
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
    } else if (pad.touched(SCREEN_PAD) && !waitForRelease) {

      if (b != noButton) {
        if (b != highlighted_button) {
          drawButton(highlighted_button, false);
          drawButton(b, true);
          highlighted_button = b;
        } else {
          if (pad.time() - pad.lastDownTime(SCREEN_PAD) > holdTime) {
            if (getApp(b)) {
              sound.click();
              clearScreen();
              launchOnRelease = getApp(b);
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

color_t LauncherApp::bgColor() {
  return screenColor[getCurrentScreen()];
}

const char* LauncherApp::infoBarText() {
  return infoText[getCurrentScreen()];
}

const char* LauncherApp::statusBarTitle() {
  return screenNames[getCurrentScreen()];
}

void LauncherApp::checkTimeout() {
  // if we haven't run in a while, reset to the middle screen
  if (clock.now() - lastRun > resetScreenTimeout) {
    setCurrentScreen(KEYS_SCREEN);
  }
}
