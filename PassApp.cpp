#include "BritepadShared.h"
#include "PassApp.h"

void PassApp::draw(void) {

  clearScreen();

  const int radius = 10;
  const int count = 8;
  const int start = screen.clipMidWidth() - radius*(3*count/2-1);
  for (int i = 0; i < count; i++) {
    screen.fillCircle(start + i*radius*3, screen.clipMidHeight(), radius, screen.red);
    sound.click();
    // todo use a timer instead of blocking
    delay(50);
  }
}
