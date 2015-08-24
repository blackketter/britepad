#include "Britepad.h"
#include "LauncherApp.h"
#include "Debug.h"

LauncherApp::LauncherApp(void) {
  for (int i = 0; i < totalButtons(); i++) {
    setButton(i,nil);
  }
}

void LauncherApp::begin(void) {

  screen.fillScreen(screen.black);

  for (int i = 0; i < totalButtons(); i++) {
    if (getButton(i)) {
        drawButton(i);
    }
  }
}

void LauncherApp::drawButton(int i, bool highlighted) {
  if (i == noButton || i >= totalButtons()) {
    return;
  }
  const int size = screen.width() / hbuttons;
  const int radius = size / 2 - 2;
  int x = size/2 + size*(i%hbuttons);
  int y =  size/2 + size*(i/hbuttons);

  screen.fillCircle( x, y, radius, highlighted ? screen.green : screen.blue);
  const char* name = apps[i]->name();
  screen.setTextSize(1);
  screen.setTextColor(screen.yellow);
  screen.setCursor( x - screen.measureTextH(name) / 2, y - screen.measureTextV(name)/2);
  screen.drawText(name);
//  screen.fillCircle(random(screen.width()), random(screen.height()), random(40), screen.red);

//  screen.setCursor(random(screen.width()), random(screen.height()));
//  screen.drawTextF("hello %s", "world");
}

int LauncherApp::buttonHit(int x, int y) {
  int h = x / (screen.width() / hbuttons);
  int v = y / (screen.height() / vbuttons);
  int i = v*hbuttons+h;
  if (getButton(i)) {
    return i;
  } else {
    return -1;
  }
}

void LauncherApp::setButton(int i, BPApp* b)
{
  apps[i] = b;
};

BPApp* LauncherApp::getButton(int i) {
  if ((i >= 0) && (i < totalButtons())) {
    return apps[i];
  } else {
    return nil;
  }
};

BPApp* LauncherApp::run(void) {

  BPApp* exit = nil;  // by default, don't exit

  int b = buttonHit(pad.x(),pad.y());

  if (pad.touched(SCREEN_PAD)) {

    if (b != noButton) {
      if (b != highlightedButton) {
        drawButton(highlightedButton, false);
        drawButton(b, true);
        highlightedButton = b;
      }
    } else {
      drawButton(highlightedButton, false);
      highlightedButton = noButton;
    }
  } else {

    if (pad.up(SCREEN_PAD)) {
      drawButton(highlightedButton, false);
      DEBUG_PARAM_LN("released on button", highlightedButton);

      if (highlightedButton != noButton) {
        exit = apps[b];
      } else {
        exit = DEFAULT_APP;
      }
      highlightedButton = noButton;
    }
  }

  return exit;
}

