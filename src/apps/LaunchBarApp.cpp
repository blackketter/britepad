#include "KeyboardApp.h"

class LaunchBarApp : public KeyboardApp {

  private:
    static const millis_t releaseTimeout = 1000;

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Launch Bar"; };

    void idle(KeyEvent* key) {
      if (getEnabled(KEYBOARD_MODE)) {
        static millis_t cmdHeld = 0;

          // a little logic here for Launchbar:
          // multiple taps selects running apps, tapping any other key switches apptoday
          // arrow keys work
        if (key->pressed(KEY_LAUNCHBAR)) {
          if (!cmdHeld) {
            Keyboard.press(MODIFIERKEY_LEFT_GUI);
          }
          Keyboard.press(KEY_SPACE);
          Keyboard.release(KEY_SPACE);
        } else if (key->released(KEY_LAUNCHBAR)) {
          cmdHeld = Uptime::millis();
        } else if (cmdHeld &&
                   !key->pressed(KEY_LAUNCHBAR) &&
                   (key->pressed() || (Uptime::millis()-cmdHeld > releaseTimeout))
                  ) {

          // arrow keys keep the gui key held, otherwise we release
          if ((key->pressed(KEY_UP) ||
                key->pressed(KEY_DOWN) ||
                key->pressed(KEY_LEFT) ||
                key->pressed(KEY_RIGHT)
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
