#include "KeyboardApp.h"

class LaunchBarApp : public KeyboardApp {

  private:
    static const millis_t releaseTimeout = 1000;

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Launch\nBar"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {
        static millis_t cmdHeld = 0;

          // a little logic here for Launchbar:
          // multiple taps selects running apps, tapping any other key switches app
          // arrow keys work
        if (keys.keyPressed((keycode_t)KEY_LAUNCHBAR)) {
          if (!cmdHeld) {
            Keyboard.press(MODIFIERKEY_LEFT_GUI);
          }
          Keyboard.press(KEY_SPACE);
          Keyboard.release(KEY_SPACE);

        } else if (keys.keyReleased((keycode_t)KEY_LAUNCHBAR)) {
          cmdHeld = Uptime::millis();
        } else if (cmdHeld &&
                   !keys.keyPressed((keycode_t)KEY_LAUNCHBAR) &&
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
    }
};

LaunchBarApp theLaunchBarApp;
