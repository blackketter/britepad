#include "KeyEventQueue.h"
#include "BritepadShared.h"

KeyEventQueue keyEvents;

void repeatTimerCallback(void* kq) { ((KeyEventQueue*)kq)->repeat(); };

KeyEventQueue::KeyEventQueue() {
  _repeatTimer.setMillis(_repeatInterval, repeatTimerCallback, this, true);
}

keyswitch_t KeyEventQueue::sendKeys() {
  KeyEvent* event = getNextEvent();
  keyswitch_t count = 0;

  while (event) {
    sendKey(event->code(), event->pressed());
    event = getNextEvent();
    count++;
  }
  return count;
}

void KeyEventQueue::releaseKeys() {
  KeyEvent* k = lastEvent();
  while (k) {
    if (keyIsDown(k->code())) {
      Keyboard.release(k->code());
    }
    k = prevEvent(k);
  }
}

void KeyEventQueue::sendKey(keycode_t code, boolean pressed) {
  if (!isSoftKeyCode(code)) {
    if (pressed) {
      Keyboard.press(code);
      //console.debugf("key press[%d]\n", code);
    } else {
      Keyboard.release(code);
      //console.debugf("key release[%d]\n", code);
    }
  } else {
    switch (code) {
      case KEY_MOUSE_MOVE_UP:
        if (pressed) {
          if (_mouseUpDownAccel >= 0) {
            _mouseUpDownAccel = -10;
          } else if (_mouseUpDownAccel > -100) {
            _mouseUpDownAccel += _mouseUpDownAccel / 10;
          }
          Mouse.move(0, _mouseUpDownAccel);
        } else {
          _mouseUpDownAccel = 0;
        }
        break;
      case KEY_MOUSE_MOVE_DOWN:
        if (pressed) {
          if (_mouseUpDownAccel <= 0) {
            _mouseUpDownAccel = 10;
          } else if (_mouseUpDownAccel < 100) {
            _mouseUpDownAccel += _mouseUpDownAccel / 10;
          }
          Mouse.move(0, _mouseUpDownAccel);
        } else {
          _mouseUpDownAccel = 0;
        }
        break;
      case KEY_MOUSE_MOVE_LEFT:
        if (pressed) {
          if (_mouseLeftRightAccel >= 0) {
            _mouseLeftRightAccel = -10;
          } else if (_mouseLeftRightAccel > -100) {
            _mouseLeftRightAccel += _mouseLeftRightAccel / 10;
          }
          Mouse.move(_mouseLeftRightAccel, 0);
        } else {
          _mouseLeftRightAccel = 0;
        }
        break;
      case KEY_MOUSE_MOVE_RIGHT:
        if (pressed) {
          if (_mouseLeftRightAccel <= 0) {
            _mouseLeftRightAccel = 10;
          } else if (_mouseLeftRightAccel < 100) {
            _mouseLeftRightAccel += _mouseLeftRightAccel / 10;
          }
          Mouse.move(_mouseLeftRightAccel, 0);
        } else {
          _mouseLeftRightAccel = 0;
        }
        break;
      case KEY_MOUSE_BUTTON_LEFT:
        pressed ? Mouse.press(MOUSE_LEFT) : Mouse.release(MOUSE_LEFT);
        break;
      case KEY_MOUSE_BUTTON_MIDDLE:
        pressed ? Mouse.press(MOUSE_MIDDLE) : Mouse.release(MOUSE_MIDDLE);
        break;
      case KEY_MOUSE_BUTTON_RIGHT:
        pressed ? Mouse.press(MOUSE_RIGHT) : Mouse.release(MOUSE_RIGHT);
        break;
      case KEY_MOUSE_BUTTON_BACK:
        pressed ? Mouse.press(MOUSE_BACK) : Mouse.release(MOUSE_BACK);
        break;
      case KEY_MOUSE_BUTTON_FORWARD:
        pressed ? Mouse.press(MOUSE_FORWARD) : Mouse.release(MOUSE_FORWARD);
        break;
      case KEY_MOUSE_SCROLL_UP:
        if (pressed) Mouse.scroll(-1, 0);
        break;
      case KEY_MOUSE_SCROLL_DOWN:
        if (pressed) Mouse.scroll(1, 0);
        break;
      case KEY_MOUSE_SCROLL_LEFT:
        if (pressed) Mouse.scroll(0, -1);
        break;
      case KEY_MOUSE_SCROLL_RIGHT:
        if (pressed) Mouse.scroll(0, 1);
        break;
      case MODIFIERKEY_MOUSE:
        break;
      case MODIFIERKEY_MOUSE_SCROLL:
        break;
      default:
        // ignore
        break;

    }
  }
}

const keycode_t mouseRepeatKeys[] = {
  KEY_MOUSE_MOVE_UP,
  KEY_MOUSE_MOVE_DOWN,
  KEY_MOUSE_MOVE_LEFT,
  KEY_MOUSE_MOVE_RIGHT,
  KEY_MOUSE_SCROLL_UP,
  KEY_MOUSE_SCROLL_DOWN,
  KEY_MOUSE_SCROLL_LEFT,
  KEY_MOUSE_SCROLL_RIGHT,
  NO_CODE
};

void KeyEventQueue::repeat() {
  int i = 0;
  keycode_t c = mouseRepeatKeys[i];
  millis_t now = Uptime::millis();
  while (c != NO_CODE) {
    KeyEvent* e = lastEvent(c);
    if (e && e->pressed() && ((now - e->time() > _repeatStart) || ((now - _lastRepeat) < _repeatStart))) {
      //console.debugf(" repeating code:%d\n",c);
      if (!e->isMouseMoveKey()) { sendKey(c,0); }
      sendKey(c,1);
      _lastRepeat = now;
    }
    c = mouseRepeatKeys[i++];
  }
}

void KeyEventQueue::truncateHistory() {
  KeyEvent* curr = _events;

  // find the nth event
  for (int i = 0; i < _maxEventHistory; i++) {
    if (curr == nullptr) {
      //console.debugln("not enough events");
      return;
    }
    curr = curr->getPrev();
  }

  // if there is one, remove the reference to it to truncate
  if (curr) {
    curr->getNext()->setPrev(nullptr);
  }

  // delete it and all the ones after it
  while (curr) {
    KeyEvent* last = curr;
    curr = curr->getPrev();
    delete last;
    //console.debugln("deleted old event");
  }
}

KeyEvent* KeyEventQueue::peekNextEvent() {
  KeyEvent* next = _lastEvent;
  if (next == nullptr) {
    next = firstEvent();
  } else {
    next = _lastEvent->getNext();
  }
  return next;
}

KeyEvent* KeyEventQueue::getNextEvent() {
  KeyEvent* next = peekNextEvent();
  if (next) {
    _lastEvent = next;
  }
  return next;
}

bool keyDebug = false;

class KeyDebugCommand : public Command {
  public:
    const char* getName() { return "keys"; }
    const char* getHelp() { return "toggle key event debug output"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      keyDebug = !keyDebug;
      c->printf("Key event debug: %s", keyDebug ? "enabled\n" : "disabled\n");
    }
};
KeyDebugCommand theKeyDebugCommand;

void KeyEventQueue::addEvent(keycode_t c, bool d) {
  addEvent(nullptr, NO_KEY, c, Uptime::millis(), d);
}

void KeyEventQueue::addEvent(KeyMatrix* m, keyswitch_t k, keycode_t c, millis_t t, bool d) {
// note that this may be called recursively

/*
  if (d && keyIsDown(c)) {
    console.debugln(" ignoring duplicate key down event");
    return;
  } else if (!d && keyIsUp(c)) {
    console.debugln(" ignoring duplicate key up event");
    return;
  }
*/

  char ch = getKeyChar(c);
  KeyEvent* e = new KeyEvent(m, k,c,ch,t,d);
  if (keyDebug) { console.debugf("key addEvent: \"%s\", switch: %d, code: %d, char: %c, pressed: %d, %s\n",
        m->getKeyLabel(c), k, c, ch, d, m == nullptr ? "soft" : "hard"); }
  if (_events) {
    _events->setNext(e);
    e->setPrev(_events);
  }

  _events = e;

  if (britepad.event(e))  {
    removeEvent(e);
  }

  //console.debugln("truncating");
  truncateHistory();

  //console.debugln("done addEvent");

}

void KeyEventQueue::removeEvent(KeyEvent* k) {
  if (keyDebug) {
    console.debugf("key removeEvent: switch: %d, code: %d, char: %c, pressed: %d, %s\n",
                k->keyswitch(), k->code(), k->character(), k->pressed(), k->matrix() == nullptr ? "soft" : "hard");
  }
  KeyEvent* curr = _events;
  while (curr) {
    if (curr == k) {
      KeyEvent* prev = k->getPrev();
      KeyEvent* next = k->getNext();
      if (prev) {
        prev->setNext(next);
      }
      if (next) {
        next->setPrev(prev);
      }
      if (_events == k) {
        _events = prev;
      }
      if (_lastEvent == k) {
        _lastEvent = next;
      }
      delete k;
      curr = nullptr;
    } else {
      curr = curr->getPrev();
    }
  }
};


bool KeyEventQueue::keyTapped(keycode_t c) {
  KeyEvent* h0 = history(0);
  KeyEvent* h1 = history(1);
  if (h0 && h1 &&
        (h1->code() == c) && h1->pressed() &&
        (h0->code() == c) && h0->released() &&
        ((h0->time() - h1->time()) < _tappedTime)
     ) {
    return true;
  } else {
    return false;
  }
}

bool KeyEventQueue::keyTapHeld(keycode_t c) {
  KeyEvent* h0 = history(0);
  KeyEvent* h1 = history(1);
  KeyEvent* h2 = history(2);
  if (h0 && h1 && h2) {
    if (
        (h2->code() == c) && h2->pressed() &&
        (h1->code() == c) && h1->released() &&
        (h0->code() == c) && h0->pressed() &&
        ((h0->time() - h2->time()) < _doubleTappedTime)
       ) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

bool KeyEventQueue::keyDoubleTapped(keycode_t c) {
  KeyEvent* h0 = history(0);
  KeyEvent* h2 = history(2);
  KeyEvent* h3 = history(3);
  if ( h0 && h2 && h3 &&
         keyTapped(c) &&
        (h3->code() == c) && h3->pressed() &&
        (h2->code() == c) && h2->released() &&
        (h0->time() - h3->time() < _doubleTappedTime)
     ) {
    return true;
  } else {
    return false;
  }
}


char KeyEventQueue::getKeyChar(keycode_t c) {
  const keyinfo_t* info = keys.getKeyInfo(c);
  if (info) {
    bool shifted = keyIsDown(MODIFIERKEY_LEFT_SHIFT) || keyIsDown(MODIFIERKEY_RIGHT_SHIFT);
    if (shifted) {
      int i = 0;
      while (shiftedKeys[i].code != NO_CODE) {
        if (shiftedKeys[i].code == c) {
          return shiftedKeys[i].c;
        }
        i++;
      }
    }
    return info->c;
  } else {
    return 0;
  }
}

class KeyEventsCommand : public Command {
  public:
    const char* getName() { return "events"; }
    const char* getHelp() { return "display keyboard events"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      keyEvents.printStatus(c);
    }
};

KeyEventsCommand theKeyEventsCommand;

void KeyEventQueue::printStatus(Stream* c) {
  if (c == nullptr) { c = &console; }
  c->println("---------------");
  c->println("Key Events:");
  KeyEvent* event = _events;
  int i = 0;
  while (event) {
    c->printf("  Key event[%2d] =%8d.%03d '%s' code:%d switch:%d %s %s\n",
      i, ((int)(event->time()))/1000, ((int)(event->time()))%1000,
      keys.getKeyLabel(event->code()),event->code(), event->keyswitch(),
      event->pressed() ? "down" : "up", event->matrix() ? "hard" : "soft");
    event = event->getPrev();
    i++;
  }
  c->println("---------------");
}

