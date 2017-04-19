#include "BritepadShared.h"
#include "KeyboardMatrix.h"
#include "ErgodoxLayout.h"


KeyboardMatrix::KeyboardMatrix() {
  setLayout(nullptr);  // set to default layout
}

// Port A is columns, Port B is rows.  Diodes have cathodes (positive) on A
void KeyboardMatrix::begin() {
  leftMatrix.SetAddress(leftAddr);
  leftMatrix.SetDirection(0, 0xff);  // port a output, port b input
  leftMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  leftMatrix.SetPortA(0); // check all columns at once

  rightMatrix.SetAddress(rightAddr);
  rightMatrix.SetDirection(0, 0xff);  // port a output, port b input
  rightMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  rightMatrix.SetPortA(0); // check all columns at once
}

void KeyboardMatrix::scanMatrix() {

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
keyswitch_t KeyboardMatrix::keysChanged() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyChanged(i)) {
      count++;
    }
  }
  return count;
}

keyswitch_t KeyboardMatrix::keysPressed() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyPressed(i)) {
      count++;
    }
  }
  return count;
}

keyswitch_t KeyboardMatrix::keysReleased() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  for (keyswitch_t i = 0; i < total; i++) {
    if (keyReleased(i)) {
      count++;
    }
  }
  return count;
}


keyswitch_t KeyboardMatrix::update() {

  millis_t now = Uptime::millis();
  if ((now - lastScan) < minScanInterval) {
    return false;
  } else {
    lastScan = now;
  }
  scanMatrix();

  return keysChanged();
}

keyswitch_t KeyboardMatrix::getSwitchFromCode(keycode_t c) {
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

keycode_t KeyboardMatrix::getCodeFromSwitch(keyswitch_t k) {
  keyswitch_t i = 0;
  while (currentLayout[i].key != NO_KEY) {
    if (currentLayout[i].key == k) {
      return currentLayout[i].code;
    } else {
      i++;
    }
  }
  return NO_CODE;
}

void KeyboardMatrix::clearKeyChanges() {
  // save the last state
  for (int i = 0; i < numColumns; i++) {
    changedKeys[i] = 0;
  }
}

void KeyboardMatrix::sendKeys() {
  int changeCount = keysChanged();
  int changeIndex = 0;
  for (keyswitch_t i = 0; i < numKeys(); i++) {
    if (keyChanged(i)) {
      bool down = isKeyDown(i);
      keycode_t code = getCodeFromSwitch(i);

      if (down) {
        Keyboard.press(code);
      } else {
        Keyboard.release(code);
      }
      console.debugf(" %d/%d - key %d %s\n", changeIndex++, changeCount, code, down ? "down" : "up" );
    }
  }
}