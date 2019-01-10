#include "BritepadShared.h"
#include "BB1KeyMatrix.h"
#include "BB1Layout.h"


BB1KeyMatrix::BB1KeyMatrix() {
   _defaultLayout = BB1Layout;
   _defaultMap = BB1Map;
  setMap();
  setLayout();
}

// Diodes have cathodes (positive) on rows, anodes on columns
void BB1KeyMatrix::begin() {
  KeyMatrix::begin();
  for (int i = 0; i < _numRows;i++) {
    pinMode(_rowPin[i], OUTPUT);
    digitalWrite(_rowPin[i], HIGH);
  }

  for (int i = 0; i < _numColumns; i++) {
    pinMode(_columnPin[i], INPUT_PULLUP);
  }
}

void BB1KeyMatrix::scanMatrix() {
  for (int i = 0; i < _numColumns; i++) {
    _curState[i] = 0;
  }

  for (int i = 0; i < _numRows; i++) {
    digitalWrite(_rowPin[i], LOW);
    delayMicroseconds(500);
    for (int j = 0; j < _numColumns; j++) {
      // active low
      if (!digitalRead(_columnPin[j])) {
        _curState[j] = _curState[j] | (0x01 << i);
      }
    }
    digitalWrite(_rowPin[i], HIGH);
  }

  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = _lastState[i] ^ _curState[i];
  }

  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _lastState[i] = _curState[i];
  }
}

void BB1KeyMatrix::update() {

  keyswitch_t count = 0;

  clearKeyChanges();

  millis_t now = Uptime::millis();
  if (now > _nextScan) {
    scanMatrix();

    keyswitch_t total = _numRows * _numColumns;

    for (keyswitch_t i = 0; i < total; i++) {
      if ((i != NO_KEY) && ((_changedKeys[i/_numRows] >> (i%_numRows)) & 0x01)) {
        count++;
        bool d = switchIsDown(i);
//        console.debugf("bb1 key %d %s\n", i, d ? "down" : "up");
        keyEvents.addEvent(this, i,getCode(i),now, d);
      }
    }

  }
  if (count) {
    //console.debugf("update found %d key events\n",count);
    _nextScan = now + _debounceInterval;
  } else {
    _nextScan = now + _minScanInterval;
  }
}

void BB1KeyMatrix::clearKeyChanges() {
  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = 0;
  }
}


