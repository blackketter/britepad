#include "KeyMatrix.h"
#include "KeyboardApp.h"

class ShiftHoldApp;

class HoldTimer : public Timer {
  public:
    HoldTimer(ShiftHoldApp* app, keycode_t key, millis_t timeout) {
      setKey(key);
      setData(app);
      setMillis(timeout);
    }
    keycode_t getKey() { return _key; }
    void setKey(keycode_t key) { _key = key; }

    static HoldTimer* find(ShiftHoldApp* app, keycode_t key) {
      Timer* t = Timer::first();
      while (t) {
        console.debugf("found timer %d (to match %d to %d)\n",t, app, t->getData());
        if (t->getData() == (void*)app && ((HoldTimer*)t)->getKey() == key) {
          console.debugf("matches key\n");

          return (HoldTimer*)t;
        }
        t = t->next();
      }
      return nullptr;
    }

  private:
    virtual void callback() { sound.click(); console.debugln("hold timer callback"); }
    keycode_t _key;
};

class ShiftHoldApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "hosh";
    const char* name() { return "Hold Shift"; };

    void eventEarly(KeyEvent* key) {
      // is this a shiftable key?
      if (getEnabled(KEYBOARD_MODE)) {  // app enabled?
        if (key->shifted()) { // is this a key that should hold-shift
          if (key->key() != NO_KEY && // is this a hardware key event
            !keys.keyIsDown(MODIFIERKEY_LEFT_SHIFT) && !keys.keyIsDown(MODIFIERKEY_RIGHT_SHIFT)) {

            keycode_t code = key->code();

            if (key->pressed()) {
              new HoldTimer(this, code, _holdTimeout);
            } else {
              HoldTimer* t = HoldTimer::find(this,code);
              // timer still active?
              if (!t) {
                console.debugln("timer expired");
                keys.addEvent(NO_KEY, MODIFIERKEY_LEFT_SHIFT, Uptime::millis(), true);
              }

              // add new key down/up event
              keys.addEvent(NO_KEY, code, Uptime::millis(), true);
              keys.addEvent(NO_KEY, code, Uptime::millis(), false);
              if (t) {
                // cancel timer
                delete t;
              } else {
                keys.addEvent(NO_KEY, MODIFIERKEY_LEFT_SHIFT, Uptime::millis(), false);
              }

            }
            // clear this event
            key->clear();
          }
        } else {
          console.debugln("time to release the pending shift holds");

          // a non-shifted key, release shift holds
          Timer* t = Timer::first();
          while (t) {
            Timer* next = t->next();
            if (t->getData() == this) {
              console.debugf("found one! releasing: %d\n", ((HoldTimer*)t)->getKey());
              keys.addEvent(NO_KEY, ((HoldTimer*)t)->getKey(), Uptime::millis(), true);
              keys.addEvent(NO_KEY, ((HoldTimer*)t)->getKey(), Uptime::millis(), false);
              delete t;
            }
            t = next;
          }
        }
      }
    };

  private:
    static const millis_t _holdTimeout = 250;
};


ShiftHoldApp theShiftHoldApp;
