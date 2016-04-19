#include "BritepadShared.h"
#include "PassApp.h"

#include "Passwords.h"

PassApp thePassApp1(PASSWORD_APP_NAME_1, PASSWORD_APP_PASS_1);
PassApp thePassApp2(PASSWORD_APP_NAME_2, PASSWORD_APP_PASS_2);

void PassApp::draw() {

  clearScreen();

  const int radius = 10;
  const int count = 8;
  const int start = screen.clipMidWidth() - radius*(3*count/2-1);
  for (int i = 0; i < count; i++) {
    screen.fillCircle(start + i*radius*3, screen.clipMidHeight(), radius, screen.red);
//    sound.click();
    // todo use a timer instead of blocking
    delay(50);
  }
}
