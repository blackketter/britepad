#include "BritepadShared.h"
#include "KeyboardMatrix.h"

KeyboardMatrix::KeyboardMatrix() {
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

bool KeyboardMatrix::isKeyDown(keyswitch_t k) {
  return (curState[k/numRows] >> (k%numRows)) & 0x01;
}

bool KeyboardMatrix::wasKeyDown(keyswitch_t k) {
  return (lastState[k/numRows] >> (k%numRows)) & 0x01;
}

// returns the total number of keys in the matrix
keyswitch_t KeyboardMatrix::numKeys() {
  return numColumns * numRows;
}

// returns the number of keys that changed state in the last idle
keyswitch_t KeyboardMatrix::keysChanged() {
  keyswitch_t count = 0;
  keyswitch_t total = numKeys();
  keyswitch_t i;
  for (i = 0; i < total; i++) {
    if (isKeyDown(i)!=wasKeyDown(i)) {
      count++;
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

bool KeyboardMatrix::idle() {

  millis_t now = Uptime::millis();
  if (now == lastScan) {
    return false;
  } else {
    lastScan = now;
  }

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

  bool changed = false;
  // check to see if any buttons have changed
  for (int i = 0; i < numColumns; i++) {
    if (curState[i] != lastState[i]) {
      changed = true;
      break;
    }
  }

  keyswitch_t changeCount = keysChanged();
  // print them out
  if (changed) {
//    console.debugf("Keys Changed: %d\n", changeCount);
    for (keyswitch_t i = 0; i < changeCount; i++) {
      console.debugf(" key %d %s\n",keyChanged(i), isKeyDown(i) ? "down":"up");
    }
/*
    console.debugf("Matrix: %02x %02x %02x %02x %02x %02x %02x\n",
      curState[0], curState[1], curState[2], curState[3],
      curState[4], curState[5], curState[6] );
*/
  }
  return changed;
}