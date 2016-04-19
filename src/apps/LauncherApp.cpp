#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "Debug.h"

LauncherApp::LauncherApp() {
  for (int s = 0; s < TOTAL_SCREENS; s++) {
    for (int i = 0; i < buttons_per_screen; i++) {
      setButton(s, i,nullptr);
    }
  }


  // go through all the apps and add them to the appropriate screens
  BritepadApp* a = britepad.getNextApp();
  int p[TOTAL_SCREENS];
  for (int i = 0; i < TOTAL_SCREENS; i++) { p[i] = 0; }

  while (a) {
      for (int i = 0; i < TOTAL_SCREENS; i++) {
        if (a->isAppType(screenTypes[i])) {
          DEBUGF("Adding %s to screen %s at position %d\n", a->name(), screenNames[i], p[i]);
          setButton(i, p[i]++, a);
        }
      }
    a = britepad.getNextApp(a);
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
      DEBUGF("%s\n",name);
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
    return apps[getCurrentScreen()][i];
  } else {
    return nullptr;
  }
};


void LauncherApp::run() {

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
