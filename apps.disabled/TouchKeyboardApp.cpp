
#include "BritepadShared.h"
#include "TouchKeyboardApp.h"

TouchKeyboardApp theTouchKeyboardApp;

void TouchKeyboardApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  if (!kbd) {
    kbd = new TouchKeyboard();
  }
  if (kbd) {
    kbd->init(screen.clipLeft(), screen.clipTop() + screen.clipHeight()/5, screen.clipWidth(), screen.clipHeight()*4/5,screen.cyan, &Arial_14_Bold, screen.black);
    kbd->draw();
  } else {
    console.debugf("failed to allocate TouchKeyboard");
  }
}

void TouchKeyboardApp::end() {
  if (kbd) {
    delete kbd;
    kbd = nullptr;
  }
  BritepadApp::end();
}

void TouchKeyboardApp::run() {
  if (!kbd) {
    return;
  }

  kbd->track();
  uint8_t d = kbd->down();
  uint8_t u = kbd->up();
  if (d) {
    console.debugf("keyboard down: %d (%c)\n", (char)d, d);
  }
  if (u) {
    console.debugf("keyboard up: %d (%c)\n", (char)u, u);
  }
}
