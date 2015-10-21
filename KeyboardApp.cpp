
#include "BritepadShared.h"
#include "KeyboardApp.h"
#include "Debug.h"

void KeyboardApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  kbd.init(screen.clipLeft(), screen.clipTop() + screen.clipHeight()/5, screen.clipWidth(), screen.clipHeight()*4/5,screen.cyan, Arial_14_Bold, screen.black);
  kbd.draw();
}

void KeyboardApp::run(void) {
  kbd.track();
  uint8_t d = kbd.down();
  uint8_t u = kbd.up();
  if (d) {
    DEBUG_PARAM_LN("keyboard down", (char)d);
  }
  if (u) {
    DEBUG_PARAM_LN("keyboard up", (char)u);
  }
}
