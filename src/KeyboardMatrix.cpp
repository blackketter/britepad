#include "BritepadShared.h"
#include "KeyboardMatrix.h"
#include "ErgodoxLayout.h"


KeyboardMatrix::KeyboardMatrix() {
  currentLayout = ergodoxLayout;
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

// returns the number of keys that changed state in the last update
keyswitch_t KeyboardMatrix::keysChanged() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();

  // check to see if any buttons have changed
  for (int i = 0; i < numColumns; i++) {
    if (curState[i] != lastState[i]) {
      // count them
      for (i = 0; i < total; i++) {
        if (isKeyDown(i)!=wasKeyDown(i)) {
          count++;
        }
      }
      break;
    }
  }
  return count;
}

// returns the nth key that's changed (up to the number returned by keysChanged())
keyswitch_t KeyboardMatrix::keyChanged(keyswitch_t n) {
  keyswitch_t count = n;
  keyswitch_t total = numKeys();
  keyswitch_t i;
  for (i = 0; i < total; i++) {
    if (isKeyDown(i)!=wasKeyDown(i)) {
      if (count == 0) {
        return i;
      } else {
        count--;
      }
    }
  }
  return NO_KEY;
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

void KeyboardMatrix::sendKeys() {
  static int cmdHeld = 0;
  keyswitch_t changeCount = keysChanged();
  for (keyswitch_t i = 0; i < changeCount; i++) {
    keyswitch_t key = keyChanged(i);
    bool down = isKeyDown(key);
    keycode_t code = getCodeFromSwitch(key);

    // a little logic here for Launchbar: delete key is launchbar key
    // multiple taps selects running apps, tapping any other key switches app
    // single tap then other keys select within launchbar
    if (code == KEY_DELETE) {
      if (down) {
        if (!cmdHeld) {
          Keyboard.press(MODIFIERKEY_LEFT_GUI);
        }
        cmdHeld++;
        Keyboard.press(KEY_SPACE);
        Keyboard.release(KEY_SPACE);
      }
    } else if (down) {
      if (cmdHeld) {
        Keyboard.release(MODIFIERKEY_LEFT_GUI);
        if (cmdHeld == 1) {
          Keyboard.press(code);
        }
        cmdHeld = 0;
      } else {
        Keyboard.press(code);
      }
    } else {
      Keyboard.release(code);
    }
//    console.debugf(" %d/%d - key %d %s\n", i+1, changeCount, c, isKeyDown(c) ? "down" : "up" );
  }

/*
    console.debugf("Matrix: %02x %02x %02x %02x %02x %02x %02x\n",
      curState[0], curState[1], curState[2], curState[3],
      curState[4], curState[5], curState[6] );
*/
}