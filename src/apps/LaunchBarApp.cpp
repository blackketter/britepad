#include "LaunchBarApp.h"

LaunchBarApp theLaunchBarApp;

void LaunchBarApp::idle() {
  static millis_t cmdHeld = 0;

    // a little logic here for Launchbar: delete key is launchbar key
    // multiple taps selects running apps, tapping any other key switches app
    // single tap then other keys select within launchbar
  if (keyMatrix.keyPressed((keycode_t)KEY_DELETE)) {
     if (!cmdHeld) {
      Keyboard.press(MODIFIERKEY_LEFT_GUI);
    }
    cmdHeld = Uptime::millis();
    Keyboard.press(KEY_SPACE);
    Keyboard.release(KEY_SPACE);
    keyMatrix.clearKeyChange((keycode_t)KEY_DELETE);

  } else if (cmdHeld &&
      (keyMatrix.keysPressed() || (Uptime::millis()-cmdHeld > releaseTimeout))) {
    // any other key press, we'll escape out
    Keyboard.release(MODIFIERKEY_LEFT_GUI);
    cmdHeld = 0;
  }
}
