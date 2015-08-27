#include "Britepad.h"
#include "PassApp.h"


BPApp* PassApp::run(void) {
  screen.fillScreen(screen.black);

  Keyboard.print(pass());

  const int radius = 10;
  const int count = 8;
  const int start = screen.width()/2 - radius*3*count/2;
  for (int i = 0; i < count; i++) {
    screen.fillCircle(start + i*radius*3, screen.height()/2, radius, screen.red);
    delay(50);
  }

  return DEFAULT_APP;
}
