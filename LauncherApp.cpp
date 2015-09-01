#include "Britepad.h"
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

  // if we haven't run in a while, reset to the middle screen
  if (now() - lastRun > resetScreenTimeout) {
    current_screen = 1;
  }

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
  const int size = screen.width() / h_buttons;
  const int radius = size / 2 - 3;
  int x = size/2 + size*(i%h_buttons);
  int y =  size/2 + size*(i/h_buttons);

  screen.fillCircle( x, y, radius, highlighted ? screen.green : apps[current_screen][i]->buttonColor());
  const char* name = apps[current_screen][i]->name();
  screen.setTextSize(1);
  screen.setTextColor(screen.black);
  screen.setCursor( x - screen.measureTextH(name) / 2, y - screen.measureTextV(name)/2);
  screen.drawText(name);
//  screen.fillCircle(random(screen.width()), random(screen.height()), random(40), screen.red);

//  screen.setCursor(random(screen.width()), random(screen.height()));
//  screen.drawTextF("hello %s", "world");
}

int LauncherApp::buttonHit(int x, int y) {
  int h = x / (screen.width() / h_buttons);
  int v = y / (screen.height() / v_buttons);
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
    return apps[current_screen][i];
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
        BritepadApp* launched = apps[current_screen][b];
        if (launched->isPopup()) {
          launched->run();
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
    if (current_screen > 0) {
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
    if (current_screen < total_screens - 1) {
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

color_t LauncherApp::bgColor(void) {
  switch (current_screen) {
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
