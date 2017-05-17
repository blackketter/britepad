#include "KeyboardApp.h"

class LaunchBarApp : public KeyboardApp {

  private:
    static const millis_t releaseTimeout = 1000;

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Launch Bar"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {
        static millis_t cmdHeld = 0;

          // a little logic here for Launchbar:
          // multiple taps selects running apps, tapping any other key switches apptoday
          // arrow keys work
        if (keys.keyPressed(KEY_LAUNCHBAR)) {
          if (!cmdHeld) {
            Keyboard.press(MODIFIERKEY_LEFT_GUI);
          }
          Keyboard.press(KEY_SPACE);
          Keyboard.release(KEY_SPACE);
        } else if (keys.keyReleased(KEY_LAUNCHBAR)) {
          cmdHeld = Uptime::millis();
        } else if (cmdHeld &&
                   !keys.keyPressed(KEY_LAUNCHBAR) &&
                   (keys.keysPressed() || (Uptime::millis()-cmdHeld > releaseTimeout))
                  ) {

          // arrow keys keep the gui key held, otherwise we release
          if ((keys.keyPressed(KEY_UP) ||
                keys.keyPressed(KEY_DOWN) ||
                keys.keyPressed(KEY_LEFT) ||
                keys.keyPressed(KEY_RIGHT)
              )) {
            cmdHeld = Uptime::millis();
          } else {
            Keyboard.release(MODIFIERKEY_LEFT_GUI);
            cmdHeld = 0;
          }
        }
      }
    }
};

LaunchBarApp theLaunchBarApp;
