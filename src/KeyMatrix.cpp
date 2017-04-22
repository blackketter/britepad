#include "BritepadShared.h"
#include "KeyMatrix.h"
#include "ErgodoxLayout.h"


KeyMatrix::KeyMatrix() {
  setLayout();  // set to default layout
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