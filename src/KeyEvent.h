#ifndef _KeyEvent_
#define _KeyEvent_

#include "KeyInfo.h"

class KeyEvent {
  public:
    KeyEvent(keyswitch_t key, keycode_t code, millis_t time, bool pressed) {
      _time = time;
      _key = key;
      _code = code;
      _pressed = pressed;
      _prev = nullptr;
      _next = nullptr;
    }

    millis_t time() { return _time; }
    keyswitch_t key() { return _key; }
    keycode_t code() { return _code; }
    bool pressed() { return _pressed; }
    bool pressed(keycode_t c) { return _pressed && (c==_code); }
    bool released() { return !_pressed; }
    bool released(keycode_t c) { return !_pressed && (c==_code); }
    KeyEvent* getNext() { return _next; }
    KeyEvent* getPrev() { return _prev; }
    void setPrev(KeyEvent* prev) { _prev = prev; }
    void setNext(KeyEvent* next) { _next = next; }
    void clear() { _code = NO_CODE; _key = NO_KEY; }

    modifierkey_t isModifier() {
      int i = 0;
      do {
        if (keyInfo[i].code == _code) {
          return keyInfo[i].modifier;
        }
        i++;
      } while (keyInfo[i].code != NO_CODE);
      return false;
    }

    bool isMouseKey() {
      return _code >= MIN_MOUSE_KEY && _code <= MAX_MOUSE_KEY;
    }

  private:
    millis_t _time;
    keyswitch_t _key;
    keycode_t _code;
    bool _pressed;
    KeyEvent* _prev;
    KeyEvent* _next;
};

#endif