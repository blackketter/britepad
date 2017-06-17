#include "BritepadShared.h"
#include "KeyMatrix.h"
#include "ErgodoxLayout.h"
#include "KeyInfo.h"

void repeatTimerCallback(void* km) { ((KeyMatrix*)km)->repeat(); };

KeyMatrix::KeyMatrix(const keymap_t* defaultMap, const keylayout_t* defaultLayout ) {
  _defaultLayout = defaultLayout;
  _defaultMap = defaultMap;
  setMap();  // set to default map
  setLayout(); // set to default layout
  _repeatTimer.setMillis(_repeatInterval, repeatTimerCallback, this, true);
}

// Port A is columns, Port B is rows.  Diodes have cathodes (positive) on A
void KeyMatrix::begin() {
  _leftMatrix.SetAddress(_leftAddr);
  _leftMatrix.SetDirection(0, 0xff);  // port a output, port b input
  _leftMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  _leftMatrix.SetPortA(0); // check all columns at once

  _rightMatrix.SetAddress(_rightAddr);
  _rightMatrix.SetDirection(0, 0xff);  // port a output, port b input
  _rightMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  _rightMatrix.SetPortA(0); // check all columns at once
}

void KeyMatrix::scanMatrix() {

  // check to see if any button is down
  uint8_t l = _leftMatrix.GetPortB();
  l = ~l;

  // we have a button down (or there's no controller)
  if (l != 0 && l != 255) {
    // scan through each column
    uint8_t col = 0x01;
    for (int i = 0; i < _numColumnsPerMatrix; i++) {
      _leftMatrix.SetPortA(~col);
      uint8_t b = _leftMatrix.GetPortB();
      _curState[i] = ~b;
      col = col << 1;
    }
    _leftMatrix.SetPortA(0); // be ready to check all columns at once
  } else {
    // no keys are pressed
    for (int i = 0; i < _numColumnsPerMatrix; i++) {
      _curState[i] = 0;
    }
  }

  uint8_t r = _rightMatrix.GetPortB();
  r = ~r;
  // we have a button down (or there's no controller)
  if (r != 0 && r != 255) {
    // scan through each column
    uint8_t col = 0x01;
    for (int i = 0; i < _numColumnsPerMatrix; i++) {
      _rightMatrix.SetPortA(~col);
      uint8_t b = _rightMatrix.GetPortB();
      _curState[i+_numColumnsPerMatrix] = ~b;
      col = col << 1;
    }
    _rightMatrix.SetPortA(0); // be ready to check all columns at once
  } else {
    // no keys are pressed
    for (int i = 0; i < _numColumnsPerMatrix; i++) {
      _curState[i+_numColumnsPerMatrix] = 0;
    }
  }

  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = _lastState[i] ^ _curState[i];
  }

  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _lastState[i] = _curState[i];
  }
}

keyswitch_t KeyMatrix::update() {

  clearKeyChanges();

  millis_t now = Uptime::millis();
  if (now - _lastScan > _minScanInterval) {
    _lastScan = Uptime::millis();
    scanMatrix();

    keyswitch_t count = 0;
    keyswitch_t total = numKeys();

    for (keyswitch_t i = 0; i < total; i++) {
      if ((i != NO_KEY) && ((_changedKeys[i/_numRows] >> (i%_numRows)) & 0x01)) {
        count++;
        bool d = switchIsDown(i);
        addEvent(i,getCode(i),_lastScan, d);
      }
    }

    if (count) {
      //console.debugf("update found %d key events\n",count);
    }
    return count;
  } else {
    return 0;
  }
}

keycode_t KeyMatrix::lookupOverlay(keycode_t c) {
  keycode_t to = ANY_CODE;
  if (_currentOverlay) {
    int i = 0;
    to = _currentOverlay[i].to;
    while (_currentOverlay[i].from != NO_CODE) {
      if (_currentOverlay[i].from == c) {
        break;
      }
      i++;
      to = _currentOverlay[i].to;
    }
  }
  if (to == ANY_CODE) { to = c; }
  return to;
}

keyswitch_t KeyMatrix::getSwitch(keycode_t c) {
  keyswitch_t i = 0;
  while (_currentMap[i].key != NO_KEY) {
    if (_currentMap[i].code == c) {
      return _currentMap[i].key;
    } else {
      i++;
    }
  }
  return NO_KEY;
}

keycode_t KeyMatrix::getCode(keyswitch_t k) {
  keyswitch_t i = 0;
  while (_currentMap[i].key != NO_KEY) {
    if (_currentMap[i].key == k) {
      return lookupOverlay(_currentMap[i].code);
    } else {
      i++;
    }
  }
  return NO_CODE;
}

const keylayout_t* KeyMatrix::getKeyLayout(keyswitch_t k) {
  keyswitch_t i = 0;
  while (_currentLayout[i].key != NO_KEY) {
    if (_currentLayout[i].key == k) {
      return (&_currentLayout[i]);
    }
    i++;
  }
  return nullptr;
}

uint8_t KeyMatrix::getWidth() {
  uint8_t maxWidth = 0;
  keyswitch_t i = 0;
  const keylayout_t* l = getDefaultLayout();
  while (l[i].key != NO_KEY) {
    uint8_t w = l[i].x+l[i].w;
    if (w > maxWidth) { maxWidth = w; }
    i++;
  }
  return maxWidth;
}

uint8_t KeyMatrix::getHeight() {
  uint8_t maxHeight = 0;
  keyswitch_t i = 0;
  const keylayout_t* l = getDefaultLayout();
  while (l[i].key != NO_KEY) {
    uint8_t h = l[i].y+l[i].h;
    if (h > maxHeight) { maxHeight = h; }
    i++;
  }
  return maxHeight;
}

uint8_t KeyMatrix::getKeyX(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->x;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyY(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->y;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyWidth(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->w;
  }
  return 0;
}
uint8_t KeyMatrix::getKeyHeight(keyswitch_t k) {
  const keylayout_t* l = getKeyLayout(k);
  if (l) {
    return l->h;
  }
  return 0;
}

int KeyMatrix::getKeyInfoIndex(keycode_t c) {
  int i = 0;
  while (keyInfo[i].code != NO_CODE) {
    if (keyInfo[i].code == c) {
      return i;
    }
    i++;
  }
  return i;
}

const keyinfo_t* KeyMatrix::getKeyInfo(keycode_t c) {

  int i = getKeyInfoIndex(c);

  if (keyInfo[i].code == NO_CODE) {
    return nullptr;
  } else {
    return &keyInfo[i];
  }
}

char KeyMatrix::getKeyChar(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
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

const icon_t KeyMatrix::getKeyIcon(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->icon;
  } else {
    return 0;
  }
}

modifierkey_t KeyMatrix::getKeyModifier(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->modifier;
  } else {
    return false;
  }
}

const char* KeyMatrix::getKeyLabel(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
    return info->label;
  } else {
    return 0;
  }
}

void KeyMatrix::clearKeyChanges() {
  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = 0;
  }
}

KeyEvent* KeyMatrix::getNextEvent() {
  KeyEvent* next = _lastEvent;
  if (next == nullptr) {
    next = firstEvent();
  } else {
    next = _lastEvent->getNext();
  }
  if (next) {
    _lastEvent = next;
    britepad.event(next);
  }
  return next;
}

keyswitch_t KeyMatrix::sendKeys() {
  KeyEvent* event = getNextEvent();
  keyswitch_t count = 0;

  while (event) {
    sendKey(event->code(), event->pressed());
    event = getNextEvent();
    count++;
  }
  return count;
}

void KeyMatrix::sendKey(keycode_t code, boolean pressed) {
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

void KeyMatrix::repeat() {
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

void KeyMatrix::truncateHistory() {
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

void KeyMatrix::setLayout(const keylayout_t* l) {
  _currentLayout = l ? l : getDefaultLayout();
}

void KeyMatrix::setMap(const keymap_t* l) {
  _currentMap = l ? l : getDefaultMap();
}

void KeyMatrix::addEvent(keyswitch_t k, keycode_t c, millis_t t, bool d) {
  //console.debugf("addEvent: switch: %d, code: %d, pressed: %d\n",k,c,d);
  KeyEvent* e = new KeyEvent(k,c,t,d);
  if (_events) {
    _events->setNext(e);
    e->setPrev(_events);
  }
  _events = e;
  //console.debugln("idling on new event");
  britepad.eventEarly(e);

  //console.debugln("truncating");
  truncateHistory();

  //console.debugln("done addEvent");

}

bool KeyMatrix::keyTapped(keycode_t c) {
  KeyEvent* h0 = history(0);
  KeyEvent* h1 = history(1);
  if (h0 && h1 &&
        (h1->code() == c) && h1->pressed() &&
        (h0->code() == c) && h0->released() &&
        (h0->time() - h1->time() < _tappedTime)
     ) {
    return true;
  } else {
    return false;
  }
}

bool KeyMatrix::keyDoubleTapped(keycode_t c) {
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

class KeysCommand : public Command {
  public:
    const char* getName() { return "keys"; }
    const char* getHelp() { return "display status of key matrix"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      keys.printStatus(c);
    }
};
KeysCommand theKeysCommand;


void KeyMatrix::printStatus(Stream* c) {
  if (c == nullptr) { c = &console; }
  c->println("---------------");
  c->println("Keyboard Status:");
  for (keyswitch_t k = 0; k < numKeys(); k++) {
    if (switchIsDown(k)) {
      c->printf(" Key '%s' is down (switch: %d)\n", getKeyLabel(getCode(k)), k);
    }
  }
  c->println("");

  KeyEvent* event = _events;
  int i = 0;
  while (event) {
    c->printf("  Key event[%d] = '%s' code:%d switch:%d %s\n", i, getKeyLabel(event->code()),event->code(), event->key(), event->pressed() ? "down" : "up");
    event = event->getPrev();
    i++;
  }
  c->println("---------------");
}

