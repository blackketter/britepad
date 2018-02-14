#include "KeyboardApp.h"


void releaseTimerCallback(void* app) {
  Keyboard.release(MODIFIERKEY_LEFT_GUI);
}

class LaunchBarApp : public KeyboardApp {

  private:
    static const millis_t releaseTimeout = 1000;
    CallbackTimer releaseTimer;

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Launch Bar"; };

    EventPriority eventPriority() { return PRIORITY_LAST; }
    bool event(KeyEvent* key) {
      if (getEnabled(KEYBOARD_MODE)) {

          // a little logic here for Launchbar:
          // multiple taps selects running apps, tapping any other key switches apptoday
          // arrow keys work
        if (key->pressed(KEY_LAUNCHBAR)) {
          if (!releaseTimer.isRunning()) {
            Keyboard.press(MODIFIERKEY_LEFT_GUI);
          }
          Keyboard.press(KEY_SPACE);
        } else if (key->released(KEY_LAUNCHBAR)) {
          releaseTimer.setMillis(releaseTimeout, releaseTimerCallback, (void*)this);
          Keyboard.release(KEY_SPACE);
        } else if (releaseTimer.isRunning() &&
                   !key->pressed(KEY_LAUNCHBAR) &&
                   key->pressed()
                  ) {

          // arrow keys keep the gui key held, otherwise we release
          if ((key->pressed(KEY_UP) ||
                key->pressed(KEY_DOWN) ||
                key->pressed(KEY_LEFT) ||
                key->pressed(KEY_RIGHT)
              )) {
            releaseTimer.setMillis(releaseTimeout, releaseTimerCallback, (void*)this);
          } else {
            releaseTimer.cancel();
            releaseTimerCallback(this);
          }
        }
      }
      return false;
    }
};

LaunchBarApp theLaunchBarApp;
