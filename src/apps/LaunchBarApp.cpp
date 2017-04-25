#include "LaunchBarApp.h"

LaunchBarApp theLaunchBarApp;

void LaunchBarApp::idle() {
  static millis_t cmdHeld = 0;

    // a little logic here for Launchbar: delete key is launchbar key
    // multiple taps selects running apps, tapping any other key switches app
    // arrow keys work
  if (keyMatrix.keyPressed((keycode_t)KEY_DELETE)) {

    console.debugf("Launchbar activated by switch %d\n", keyMatrix.getSwitch((keycode_t)KEY_DELETE));
    keyMatrix.dumpStatus();
    if (!cmdHeld) {
      Keyboard.press(MODIFIERKEY_LEFT_GUI);
    }
    Keyboard.press(KEY_SPACE);
    Keyboard.release(KEY_SPACE);
    keyMatrix.clearKeyChange((keycode_t)KEY_DELETE);

  } else if (keyMatrix.keyReleased((keycode_t)KEY_DELETE)) {
    cmdHeld = Uptime::millis();
  } else if (cmdHeld &&
             !keyMatrix.keyPressed((keycode_t)KEY_DELETE) &&
             (keyMatrix.keysPressed() || (Uptime::millis()-cmdHeld > releaseTimeout))
            ) {

    // arrow keys keep the gui key held, otherwise we release
    if ((keyMatrix.keyPressed((keycode_t)KEY_UP) ||
          keyMatrix.keyPressed((keycode_t)KEY_DOWN) ||
          keyMatrix.keyPressed((keycode_t)KEY_LEFT) ||
          keyMatrix.keyPressed((keycode_t)KEY_RIGHT)
        )) {
          cmdHeld = Uptime::millis();
        } else {
          Keyboard.release(MODIFIERKEY_LEFT_GUI);
          cmdHeld = 0;
        }

  }
}
