#include "LaunchBarApp.h"

LaunchBarApp theLaunchBarApp;

void LaunchBarApp::idle() {
  static millis_t cmdHeld = 0;

    // a little logic here for Launchbar: delete key is launchbar key
    // multiple taps selects running apps, tapping any other key switches app
    // arrow keys work
  if (keys.keyPressed((keycode_t)KEY_DELETE)) {
    if (!cmdHeld) {
      Keyboard.press(MODIFIERKEY_LEFT_GUI);
    }
    Keyboard.press(KEY_SPACE);
    Keyboard.release(KEY_SPACE);
    keys.clearKeyChange((keycode_t)KEY_DELETE);

  } else if (keys.keyReleased((keycode_t)KEY_DELETE)) {
    cmdHeld = Uptime::millis();
  } else if (cmdHeld &&
             !keys.keyPressed((keycode_t)KEY_DELETE) &&
             (keys.keysPressed() || (Uptime::millis()-cmdHeld > releaseTimeout))
            ) {

    // arrow keys keep the gui key held, otherwise we release
    if ((keys.keyPressed((keycode_t)KEY_UP) ||
          keys.keyPressed((keycode_t)KEY_DOWN) ||
          keys.keyPressed((keycode_t)KEY_LEFT) ||
          keys.keyPressed((keycode_t)KEY_RIGHT)
        )) {
          cmdHeld = Uptime::millis();
        } else {
          Keyboard.release(MODIFIERKEY_LEFT_GUI);
          cmdHeld = 0;
        }

  }
}
