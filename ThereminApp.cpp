#include "Britepad.h"
#include "ThereminApp.h"
#include "Debug.h"

BritepadApp* ThereminApp::run(void) {

  if (pad.down(SCREEN_PAD)) {
    sound.tone( ((float)pad.x() / screen.width()) * 2 * sound.MIDDLE_C_FREQ, (float)pad.y() / screen.height());
    // clear screen and draw matrix
    screen.fillScreen(bgColor());
  } else if (pad.touched(SCREEN_PAD)) {
    sound.tone( ((float)pad.x() / screen.width()) * 2 * sound.MIDDLE_C_FREQ, (float)pad.y() / screen.height());
    // draw some traces here
    screen.fillCircle(pad.x(), pad.y(), pad.y()/10, (long)65535 * pad.x() / screen.width());
  }


  if (pad.up(SCREEN_PAD)) {
    sound.tone(0,0);
  }

  return STAY_IN_APP;
}

void ThereminApp::end(BritepadApp* nextApp) {
  BritepadApp::end(nextApp);
  sound.tone(0,0);  // stop the tone
}

