#include "BritepadShared.h"
#include "ErgodoxKeyMatrix.h"
#include "ErgodoxLayout.h"

ErgodoxKeyMatrix::ErgodoxKeyMatrix() {
   _defaultLayout = ergodoxLayout;
   _defaultMap = ergodoxMap;
  setMap();
  setLayout();
}

// Port A is columns, Port B is rows.  Diodes have cathodes (positive) on A
void ErgodoxKeyMatrix::begin(EventQueue* queue) {
  KeyMatrix::begin(queue);

  _leftMatrix.SetAddress(_leftAddr);
  _leftMatrix.SetDirection(0, 0xff);  // port a output, port b input
  _leftMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  _leftMatrix.SetPortA(0); // check all columns at once

  _rightMatrix.SetAddress(_rightAddr);
  _rightMatrix.SetDirection(0, 0xff);  // port a output, port b input
  _rightMatrix.SetPullups(0,0xff); // port a no pullups, port b pullups
  _rightMatrix.SetPortA(0); // check all columns at once
}

void ErgodoxKeyMatrix::scanMatrix() {

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

bool ErgodoxKeyMatrix::update() {

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
        _queue->addEvent(this, i,getCode(i),now, d);
      }
    }

  }
  if (count) {
    //console.debugf("update found %d key events\n",count);
    _nextScan = now + _debounceInterval;
  } else {
    _nextScan = now + _minScanInterval;
  }
  return count != 0;
}

void ErgodoxKeyMatrix::clearKeyChanges() {
  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = 0;
  }
}


