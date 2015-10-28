#include "BritepadShared.h"
#include "ThereminApp.h"
#include "Debug.h"

void ThereminApp::run() {
  long octave = 6;
  uint8_t prox = pad.getProximityDistance();
  DEBUG_PARAM_LN("prox", prox);

  if (pad.down(SCREEN_PAD)) {
    // clear screen and draw matrix
    clearScreen();
  } else if (pad.touched(SCREEN_PAD)) {
    sound.tone( ((float)pad.x() / screen.clipWidth()) * octave * sound.MIDDLE_C_FREQ, (float)pad.y() / screen.clipHeight());
    // draw some traces here
    screen.fillCircle(pad.x(), pad.y(), pad.y()/10, 65535L * pad.x() / screen.clipWidth());
  } else if (prox) {
    if (lastProx != prox) {
      lastProx = prox;
      sound.tone( (float)prox / 255 * octave * sound.MIDDLE_C_FREQ, 1.0);
      screen.fillRect(screen.clipLeft() +(long)prox * screen.clipWidth() * 19/20 / pad.getProximityMax(), screen.clipTop(), screen.clipWidth()/20, screen.clipHeight(), currColor++);
    }
  } else if (lastProx) {
    sound.tone(0,0);
    lastProx = 0;
  }
  if (pad.up(SCREEN_PAD)) {
    sound.tone(0,0);
  }
}

void ThereminApp::end(BritepadApp* nextApp) {
  BritepadApp::end(nextApp);
  sound.tone(0,0);  // stop the tone
}

