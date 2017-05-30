#include "BritepadShared.h"
#include "KeyMatrix.h"
#include "ErgodoxLayout.h"
#include "KeyInfo.h"

KeyMatrix::KeyMatrix(const keymap_t* defaultMap, const keylayout_t* defaultLayout ) {
  _defaultLayout = defaultLayout;
  _defaultMap = defaultMap;
  setMap();  // set to default map
  setLayout(); // set to default layout
  clearHistory();
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

keyswitch_t KeyMatrix::keysPressed() {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if (getHistoryPressed(i))
      count++;
  }
  return count;
}

keyswitch_t KeyMatrix::keysReleased() {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if (getHistoryReleased(i))
      count++;
  }
  return count;
}

keyswitch_t KeyMatrix::keysChanged() {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
      count++;
  }
  return count;
}

bool KeyMatrix::switchChanged(keyswitch_t k) {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if (getHistoryKey(i) == k)
      count++;
  }
  return count;
}

bool KeyMatrix::keyChanged(keycode_t c) {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if (getHistoryCode(i) == c)
      count++;
  }
  return count;
}

bool KeyMatrix::keyPressed(keycode_t c) {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if ((getHistoryCode(i) == c) && getHistoryPressed(i))
      count++;
  }
  return count;
}

bool KeyMatrix::keyReleased(keycode_t c) {
  keyswitch_t count = 0;
  for (int i = 0; i < _historySize; i++) {
    if (getHistoryTime(i) < _lastFlush)
      break;
    if ((getHistoryCode(i) == c) && getHistoryReleased(i))
      count++;
  }
  return count;
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
        addHistory(i,getCode(i),_lastScan, d);
        if (d && _click) {
          sound.click();
        }
      }
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

const keyinfo_t* KeyMatrix::getKeyInfo(keycode_t c) {
  int i = 0;
  while (keyInfo[i].code != NO_CODE) {
    if (keyInfo[i].code == c) {
      return &keyInfo[i];
    }
    i++;
  }
  return nullptr;
}

char KeyMatrix::getKeyChar(keycode_t c) {
  const keyinfo_t* info = getKeyInfo(c);
  if (info) {
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

bool KeyMatrix::getKeyModifier(keycode_t c) {
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

keyswitch_t KeyMatrix::sendKeys() {
  keyswitch_t count = 0;
  for (int i = _historySize-1;i >= 0; i--) {
    if (getHistoryTime(i) > _lastFlush) {
      //console.debugf("%d _lastflush\n", _lastFlush);
      keycode_t c = getHistoryCode(i);
      if (c != NO_CODE) {
        if (getHistoryPressed(i)) {
          Keyboard.press(c);
          //console.debugf("%d key press[%d]: %d\n", (int)getHistoryTime(i), i, c);
        } else {
          Keyboard.release(c);
          //console.debugf("%d key release[%d] %d\n", (int)getHistoryTime(i), i, c);
        }
      }
      count++;
    }
  }
  flush();
  return count;
}

void KeyMatrix::flush() {
  _lastFlush = Uptime::millis();
}

void KeyMatrix::setLayout(const keylayout_t* l) {
  _currentLayout = l ? l : getDefaultLayout();
}

void KeyMatrix::setMap(const keymap_t* l) {
  _currentMap = l ? l : getDefaultMap();
}

millis_t KeyMatrix::getHistoryTime(uint8_t n) {
  if (n < _historySize) {
    return _history[n].time;
  } else {
    return 0;
  }
}

keyswitch_t KeyMatrix::getHistoryKey(uint8_t n) {
  if (n < _historySize) {
    return _history[n].key;
  } else {
    return NO_KEY;
  }
}

keycode_t KeyMatrix::getHistoryCode(uint8_t n) {
  if (n < _historySize) {
    return _history[n].code;
  } else {
    return NO_KEY;
  }
}

bool KeyMatrix::getHistoryPressed(uint8_t n) {
  if (n < _historySize) {
    return _history[n].pressed;
  } else {
    return false;
  }
}

void KeyMatrix::addHistory(keyswitch_t k, keycode_t c, millis_t t, bool d) {
  for (uint8_t i = _historySize-1; i > 0; i--) {
    _history[i] = _history[i-1];
  }
  _history[0].pressed = d;
  _history[0].time = t;
  _history[0].key = k;
  _history[0].code = c;
  //console.debugf("%d addHistory: key: %d code: %d down: %d\n",(int)t, k,c,d);
}

void KeyMatrix::clearHistory() {
  for (uint8_t i = 0; i < _historySize; i++) {
    _history[i].key = NO_KEY;
    _history[i].code = NO_CODE;
    _history[i].time = 0;
  }
}

void KeyMatrix::deleteHistory(uint8_t n) {
  if (n < _historySize) {
    // leave the event in the queue, just make it not a key
    _history[n].code = NO_CODE;
    _history[n].key = NO_KEY;
  }
}

void KeyMatrix::deleteHistory(keycode_t c, bool pressed) {
  for (int i = 0; i < _historySize; i++) {
    if ((getHistoryPressed(i) == pressed) && (getHistoryCode(i) == c)) {
      deleteHistory(i);
      break;
    }
  }
}

bool KeyMatrix::keyTapped(keycode_t c) {
  if (  keyReleased(c) &&
        (getHistoryCode(1) == c) && getHistoryPressed(1) &&
        (getHistoryCode(0) == c) && getHistoryReleased(0) &&
        (getHistoryTime(0)-getHistoryTime(1) < _tappedTime)
     ) {
    return true;
  } else {
    return false;
  }
}

bool KeyMatrix::keyDoubleTapped(keycode_t c) {
  if ( keyTapped(c) &&
        (getHistoryCode(3) == c) && getHistoryPressed(3) &&
        (getHistoryCode(2) == c) && getHistoryReleased(2) &&
        (getHistoryTime(0)-getHistoryTime(3) < _doubleTappedTime)
     ) {
    return true;
  } else {
    return false;
  }
}

bool KeyMatrix::keyIsModifier(keycode_t c) {
  int i = 0;
  while (modifierKeys[i] != NO_CODE) {
    if (modifierKeys[i] == c) {
      return true;
    }
    i++;
  }
  return false;
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
    if (switchPressed(k)) {
      c->printf(" Key '%s' was pressed (switch: %d)\n", getKeyLabel(getCode(k)), k);
    } else if (switchIsDown(k)) {
      c->printf(" Key '%s' is down (switch: %d)\n", getKeyLabel(getCode(k)), k);
    } else if (switchReleased(k)) {
      c->printf(" Key '%s' was released (switch: %d)\n", getKeyLabel(getCode(k)), k);
    }
  }
  c->println("");
  for (uint8_t i = 0; i < _historySize; i++) {
      c->printf("  Key History[%d] = '%s' %d %s\n", i, getKeyLabel(getHistoryCode(i)),getHistoryKey(i), getHistoryPressed(i) ? "down" : "up");
  }
  c->println("---------------");
}

