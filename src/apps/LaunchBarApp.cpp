#include "KeyboardApp.h"


static void releaseTimerCallback(void* app) {
  Keyboard.release(MODIFIERKEY_LEFT_GUI);
}

class LaunchBarApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Launch Bar Del"; };

    EventPriority eventPriority() { return PRIORITY_LAST; }
    bool event(KeyEvent* key) {
      if (getEnabled(KEYBOARD_MODE) && !britepad.currentApp()->usesKeyboard()) {

          // a little logic here for Launchbar:
          // multiple taps selects running apps, tapping any other key switches apptoday
          // arrow keys work
        if (key->pressed(_launchbarKey)) {
          if (!_releaseTimer.isRunning()) {
            Keyboard.press(MODIFIERKEY_LEFT_GUI);
          }
          Keyboard.press(KEY_SPACE);
        } else if (key->released(_launchbarKey)) {
          _releaseTimer.setMillis(releaseTimeout, releaseTimerCallback, (void*)this);
          Keyboard.release(KEY_SPACE);
        } else if (_releaseTimer.isRunning() &&
                   !key->pressed(_launchbarKey) &&
                   key->pressed()
                  ) {

          // arrow keys keep the gui key held, otherwise we release
          if ((key->pressed(KEY_UP) ||
                key->pressed(KEY_DOWN) ||
                key->pressed(KEY_LEFT) ||
                key->pressed(KEY_RIGHT)
              )) {
            _releaseTimer.setMillis(releaseTimeout, releaseTimerCallback, (void*)this);
          } else {
            _releaseTimer.cancel();
            releaseTimerCallback(this);
          }
        }
        if (key->code(_launchbarKey)) {
          return true; // consume key
        }
      }
      return false;
    }

  protected:
    AppMode defaultEnabled() override { return NO_MODE; }

  private:
    static const millis_t releaseTimeout = 1000;
    CallbackTimer _releaseTimer;
    const keycode_t _launchbarKey = KEY_DELETE;

};

LaunchBarApp theLaunchBarApp;
