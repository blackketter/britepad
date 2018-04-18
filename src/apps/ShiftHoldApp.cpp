#include "KeyMatrix.h"
#include "KeyboardApp.h"

// this app lets you type shifted key by holding down the key for more than a default 250ms.

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
        if (t->getData() == (void*)app && ((HoldTimer*)t)->getKey() == key) {
          return (HoldTimer*)t;
        }
        t = t->next();
      }
      return nullptr;
    }

  private:
    virtual void callback() {
      keyEvents.addEvent(nullptr, NO_KEY, MODIFIERKEY_LEFT_SHIFT, Uptime::millis(), true);
      keyEvents.addEvent(nullptr, NO_KEY, _key, Uptime::millis(), true);
      keyEvents.addEvent(nullptr, NO_KEY, MODIFIERKEY_LEFT_SHIFT, Uptime::millis(), false);
    }
    keycode_t _key;
};

class ShiftHoldApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "hosh";
    const char* name() { return "Hold Shift"; };
    EventPriority eventPriority() { return (EventPriority)(PRIORITY_FIRST+1); }

    bool event(KeyEvent* key) {
      bool consumed = false;
      // is this a shiftable key?
      if (getEnabled(KEYBOARD_MODE) && key->hard()) {  // app enabled & not a soft key
        if (key->shifted()) { // is this a key that should hold-shift
          if (key->key() != NO_KEY && // is this a hardware key event
            !keyEvents.keyIsDown(MODIFIERKEY_LEFT_SHIFT) && !keyEvents.keyIsDown(MODIFIERKEY_RIGHT_SHIFT)) {

            keycode_t code = key->code();

            if (key->pressed()) {
              new HoldTimer(this, code, _holdTimeout);
              key->clear();
              consumed = true;
            } else {
              HoldTimer* t = HoldTimer::find(this,code);
              // timer still active?
              if (t) {
                // cancel timer
                delete t;
                // send the deferred key
                keyEvents.addEvent(nullptr,  key->key(), key->code(), Uptime::millis(), true);
                keyEvents.addEvent(nullptr,  key->key(), key->code(), Uptime::millis(), false);
                key->clear();
                consumed = true;
              }
            }
          }
        } else {
          // a non-shifted key, release shift holds
          Timer* t = Timer::first();
          bool deleted = false;
          while (t) {
            Timer* next = t->next();
            if (t->getData() == this) {
              keyEvents.addEvent(nullptr, NO_KEY, ((HoldTimer*)t)->getKey(), Uptime::millis(), true);
              delete t;
              deleted = true;
            }
            t = next;
          }
          if (deleted) {
            // requeue this event after the ones we just added.
            keyEvents.addEvent(nullptr, key->key(), key->code(), Uptime::millis(), key->pressed());
            key->clear();
            consumed = true;
          }
        }
      }
      return consumed;
    };

  private:
    static const millis_t _holdTimeout = 250;
};


ShiftHoldApp theShiftHoldApp;