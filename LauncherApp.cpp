#include "Britepad.h"
#include "LauncherApp.h"
#include "Debug.h"

void LauncherApp::begin(void) {
  DEBUG_LN("launcher app begin");
  screen.fillScreen(screen.blue);
}

void LauncherApp::end(void) {

}

void LauncherApp::run(void) {
  screen.fillCircle(random(screen.width()), random(screen.height()), random(40), screen.red);

  screen.setCursor(random(screen.width()), random(screen.height()));
  screen.drawTextF("hello %s", "world");
}

