#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "Debug.h"

#include <Time.h>

LauncherApp::LauncherApp(void) {
  for (int s = 0; s < total_screens; s++) {
    for (int i = 0; i < buttons_per_screen; i++) {
      setButton(s, i,nil);
    }
  }
}

void LauncherApp::begin(void) {

  // this should wake up the host, which is great for entering passwords
  // but might have some ugly side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  sound.swipe(DIRECTION_DOWN);
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
  const int vsize = height() / v_buttons;
  const int hsize = width() / h_buttons;

  int x = hsize/2 + hsize*(i%h_buttons) + left();
  int y = vsize/2 + vsize*(i/h_buttons) + top();

  const int radius = min(hsize,vsize) / 2 - 2;

  screen.fillCircle( x, y, radius, highlighted ? screen.mix(apps[currentScreen()][i]->buttonColor(), screen.black) : apps[currentScreen()][i]->buttonColor());
  const char* name = apps[currentScreen()][i]->name();
  screen.setTextSize(1);
  screen.setTextColor(screen.black);
  screen.setCursor( x - screen.measureTextH(name) / 2, y - screen.measureTextV(name)/2);
  screen.drawText(name);
}

int LauncherApp::buttonHit(int x, int y) {

  int h = (x - left()) / (width() / h_buttons);
  int v = (y - top()) / (height() / v_buttons);

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

  lastRun = now();

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
      DEBUG_PARAM_LN("released on button", highlighted_button);

      if (highlighted_button != noButton) {
        BritepadApp* launched = apps[currentScreen()][b];
        if (launched->isPopup()) {
          exit = launched->run();
          if (!launched->isInvisible()) {
            screen.fillScreen(bgColor());
            drawButtons();
          } else {
            drawButton(b, false);
          }
        } else {
          if (launched->isScreensaver()) {
            currentScreensaver = launched;
          }
          exit = launched;
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
      updateStatusBar(true);
      drawButtons();
    } else {
      sound.bump();
    }
  }

  if (pad.down(RIGHT_PAD)) {
    if (currentScreen() < total_screens - 1) {
      current_screen++;
      sound.swipe(DIRECTION_RIGHT);
      screen.pushFill(DIRECTION_RIGHT, bgColor());
      updateStatusBar(true);
      drawButtons();
    } else {
      sound.bump();
    }
  }

  if (pad.down(TOP_PAD)) {
      sound.swipe(DIRECTION_UP);
      exit = DEFAULT_APP;
  }

  return exit;
}

int LauncherApp::currentScreen(void) {
  // if we haven't run in a while, reset to the middle screen
  if (now() - lastRun > resetScreenTimeout) {
    current_screen = 1;
  }

  return current_screen;
}

color_t LauncherApp::bgColor(void) {

  switch (currentScreen()) {
    case 0:
      return screen.darkerred;
    case 1:
      return screen.darkergreen;
    case 2:
      return screen.darkerblue;
    default:
      return 0;
  }
}
