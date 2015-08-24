#include "Britepad.h"
#include "BPApp.h"
#include "KeyApp.h"

void KeyApp::begin(void) {
  Keyboard.print(keyStr);
  screen.fillScreen(screen.black);
  screen.setTextSize(3);
  screen.setTextColor(screen.yellow);
  int x = screen.width()/2 - (screen.measureTextH(nameStr) / 2);
  int y =  screen.height()/2 - (screen.measureTextV(nameStr) / 2);
  screen.setCursor( x, y);

  screen.drawText(nameStr);
  delay(200);
}

BPApp* KeyApp::run(void) {
  return BACK_APP;
}
