#include "BritepadShared.h"
#include "KeyMatrix.h"
#include "ErgodoxLayout.h"

class KeysCommand : public Command {
  public:
    const char* getName() { return "keys"; }
    const char* getHelp() { return "display status of key matrix"; }
    void execute(Stream* c, uint8_t paramCount, char** params) {
      keyMatrix.dumpStatus(c);
    }
};
KeysCommand theKeysCommand;

KeyMatrix::KeyMatrix() {
  setLayout();  // set to default layout
  clearHistory();
}

// Port A is columns, Port B is rows.  Diodes have cathodes (positive) on A
void KeyMatrix::begin() {
  leftMatrix.SetAddress(leftAddr);
  leftMatrix.SetDirection(0, 0xff);  // port a output, port b input
  leftMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  leftMatrix.SetPortA(0); // check all columns at once

  rightMatrix.SetAddress(rightAddr);
  rightMatrix.SetDirection(0, 0xff);  // port a output, port b input
  rightMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  rightMatrix.SetPortA(0); // check all columns at once
}

void KeyMatrix::scanMatrix() {

  // save the last state
  for (int i = 0; i < numColumns; i++) {
    lastState[i] = curState[i];
  }

  // check to see if any button is down
  uint8_t l = leftMatrix.GetPortB();
  l = ~l;

  // we have a button down (or there's no controller)
  if (l != 0 && l != 255) {
    // scan through each column
    uint8_t col = 0x01;
    for (int i = 0; i < numColumnsPerMatrix; i++) {
      leftMatrix.SetPortA(~col);
      uint8_t b = leftMatrix.GetPortB();
      curState[i] = ~b;
      col = col << 1;
    }
    leftMatrix.SetPortA(0); // be ready to check all columns at once
  } else {
    // no keys are pressed
    for (int i = 0; i < numColumnsPerMatrix; i++) {
      curState[i] = 0;
    }
  }

  uint8_t r = rightMatrix.GetPortB();
  r = ~r;
  // we have a button down (or there's no controller)
  if (r != 0 && r != 255) {
    // scan through each column
    uint8_t col = 0x01;
    for (int i = 0; i < numColumnsPerMatrix; i++) {
      rightMatrix.SetPortA(~col);
      uint8_t b = rightMatrix.GetPortB();
      curState[i+numColumnsPerMatrix] = ~b;
      col = col << 1;
    }
    rightMatrix.SetPortA(0); // be ready to check all columns at once
  } else {
    // no keys are pressed
    for (int i = 0; i < numColumnsPerMatrix; i++) {
      curState[i+numColumnsPerMatrix] = 0;
    }
  }

  for (int i = 0; i < numColumns; i++) {
    changedKeys[i] = lastState[i] ^ curState[i];
  }
}

// returns the number of keys that changed state in the last update
keyswitch_t KeyMatrix::keysChanged() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyChanged(i)) {
      count++;
      addHistory(i,Uptime::millis(), isKeyDown(i));
    }
  }
  return count;
}

keyswitch_t KeyMatrix::keysPressed() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyPressed(i)) {
      count++;
    }
  }
  return count;
}

keyswitch_t KeyMatrix::keysReleased() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyReleased(i)) {
      count++;
    }
  }
  return count;
}


keyswitch_t KeyMatrix::update() {

  millis_t now = Uptime::millis();
  if ((now - lastScan) < minScanInterval) {
    return false;
  } else {
    lastScan = now;
  }
  scanMatrix();

  return keysChanged();
}

keyswitch_t KeyMatrix::getSwitch(keycode_t c) {
  keyswitch_t i = 0;
  while (currentLayout[i].key != NO_KEY) {
    if (currentLayout[i].code == c) {
      return currentLayout[i].key;
    } else {
      i++;
    }
  }
  return NO_KEY;
}

keycode_t KeyMatrix::getCode(keyswitch_t k) {
  const keylayout_t* l = getKey(k);
  if (l) {
    return l->code;
  }
 return NO_CODE;
}

const keylayout_t* KeyMatrix::getKey(keyswitch_t k, const keylayout_t* l) {
  if (l == nullptr) { l = currentLayout; }
  keyswitch_t i = 0;
  while (l[i].key != NO_KEY) {
    if (l[i].key == k) {
      return (&l[i]);
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

// todo: make these accurate
char KeyMatrix::getChar(keyswitch_t k) { return 'x'; }

uint8_t KeyMatrix::getKeyX(keyswitch_t k) {
  const keylayout_t* l = getKey(k, getDefaultLayout());
  if (l) {
    return l->x;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyY(keyswitch_t k) {
  const keylayout_t* l = getKey(k, getDefaultLayout());
  if (l) {
    return l->y;
  }
  return 0;
}

uint8_t KeyMatrix::getKeyWidth(keyswitch_t k) {
  const keylayout_t* l = getKey(k, getDefaultLayout());
  if (l) {
    return l->w;
  }
  return 0;
}
uint8_t KeyMatrix::getKeyHeight(keyswitch_t k) {
  const keylayout_t* l = getKey(k, getDefaultLayout());
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
  for (int i = 0; i < numColumns; i++) {
    changedKeys[i] = 0;
  }
}

void KeyMatrix::sendKeys() {
  for (keyswitch_t i = 0; i < numKeys(); i++) {
    if (keyChanged(i)) {
      bool down = isKeyDown(i);
      keycode_t code = getCode(i);

      if (down) {
        Keyboard.press(code);
      } else {
        Keyboard.release(code);
      }
    }
  }
}

void KeyMatrix::setLayout(const keylayout_t* l) {
  console.debugf("setlayout: %d\n",(int)l);
  currentLayout = l ? l : getDefaultLayout();
  console.debugf(" now: %d (default: %d vs %d)\n",(int)currentLayout, (int)keyMatrix.getDefaultLayout(), (int)getDefaultLayout());
}

millis_t KeyMatrix::getHistoryTime(uint8_t n) {
  if (n < historySize) {
    return historyTime[n];
  } else {
    return 0;
  }
}

keyswitch_t KeyMatrix::getHistoryKey(uint8_t n) {
  if (n < historySize) {
    return historyKey[n];
  } else {
    return NO_KEY;
  }
}

keycode_t KeyMatrix::getHistoryCode(uint8_t n) {
  if (n < historySize) {
    return historyCode[n];
  } else {
    return NO_KEY;
  }
}

bool KeyMatrix::getHistoryPressed(uint8_t n) {
  if (n < historySize) {
    return historyPressed[n];
  } else {
    return false;
  }
}

void KeyMatrix::addHistory(keyswitch_t k, millis_t t, bool d) {
  for (uint8_t i = historySize-1; i > 0; i--) {
    historyPressed[i] = historyPressed[i-1];
    historyKey[i] = historyKey[i-1];
    historyCode[i] = historyCode[i-1];
    historyTime[i] = historyTime[i-1];
  }
  historyPressed[0] = d;
  historyTime[0] = t;
  historyKey[0] = k;
  historyCode[0] = getCode(k);
//  for (uint8_t i = 0; i < historySize; i++) {
//    console.debugf("History[%d] = %d %s\n", i, getHistoryKey(i), getHistoryPressed(i) ? "down" : "up");
//  }
}

void KeyMatrix::clearHistory() {
  for (uint8_t i = 0; i < historySize; i++) {
    historyKey[i] = NO_KEY;
    historyCode[i] = NO_CODE;
  }
}

bool KeyMatrix::doubleTapped(keyswitch_t k) {
  if (
        (getHistoryKey(3) == k) && getHistoryPressed(3) &&
        (getHistoryKey(2) == k) && getHistoryReleased(2) &&
        (getHistoryKey(1) == k) && getHistoryPressed(1) &&
        (getHistoryKey(0) == k) && getHistoryReleased(0)
     ) {
    console.debugf("%d doubleTapped!\n", k);
    return true;
  } else {
    console.debugf("%d not doubleTapped!\n", k);
    return false;
  }
}

bool KeyMatrix::doubleTapped(keycode_t c) {
  if (
        (getHistoryCode(3) == c) && getHistoryPressed(3) &&
        (getHistoryCode(2) == c) && getHistoryReleased(2) &&
        (getHistoryCode(1) == c) && getHistoryPressed(1) &&
        (getHistoryCode(0) == c) && getHistoryReleased(0) &&
        (getHistoryTime(0)-getHistoryTime(3) < doubleTapTime)
     ) {
    console.debugf("%d doubleTapped!\n", c);
    return true;
  } else {
    console.debugf("%d not doubleTapped!\n", c);
    return false;
  }
}

void KeyMatrix::dumpStatus(Stream* c) {
  if (c == nullptr) { c = &console; }
  c->println("---------------");
  c->println("Keyboard Status:");
  for (keyswitch_t k = 0; k < numKeys(); k++) {
    if (keyPressed(k)) {
      c->printf(" Key '%s' pressed (switch: %d)\n", getKeyLabel(getCode(k)), k);
    } else if (isKeyDown(k)) {
      c->printf(" Key '%s' down (switch: %d)\n", getKeyLabel(getCode(k)), k);
    } else if (keyReleased(k)) {
      c->printf(" Key '%s' released (switch: %d)\n", getKeyLabel(getCode(k)), k);
    }
  }
  c->println("");
  for (uint8_t i = 0; i < historySize; i++) {
      c->printf("  Key History[%d] = '%s' %d %s\n", i, getKeyLabel(getHistoryCode(i)),getHistoryKey(i), getHistoryPressed(i) ? "down" : "up");
  }
  c->println("---------------");
}

