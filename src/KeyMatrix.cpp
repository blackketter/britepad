#include "KeyMatrix.h"
#include "KeyEventQueue.h"
#include "BritepadShared.h"

void KeyMatrix::begin(KeyEventQueue* queue) {
  _queue = queue;
  setMap();  // set to default map
  setLayout(); // set to default layout
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

void KeyMatrix::setLayout(const keylayout_t* l) {
  _currentLayout = l ? l : getDefaultLayout();
}

void KeyMatrix::setMap(const keymap_t* l) {
  _currentMap = l ? l : getDefaultMap();
}


//////////////////////////////////////////////////
// GPIOKeyMatrix
//////////////////////////////////////////////////
GPIOKeyMatrix::GPIOKeyMatrix(const keylayout_t* keylayout, const keymap_t* keymap, uint8_t rows, uint8_t columns, const pinNumber* rowPins, const pinNumber* columnPins) {
  _numRows = rows;
  _numColumns = columns;
  _curState = new uint8_t[_numColumns];
  _lastState = new uint8_t[_numColumns];
  _changedKeys = new uint8_t[_numColumns];
  _rowPin = rowPins;
  _columnPin = columnPins;
  _defaultMap = keymap;
  _defaultLayout = keylayout;
}

GPIOKeyMatrix::~GPIOKeyMatrix() {
  delete(_curState);
  delete(_lastState);
  delete(_changedKeys);
}

// Diodes have cathodes (positive) on rows, anodes on columns
void GPIOKeyMatrix::begin(KeyEventQueue* queue) {
  KeyMatrix::begin(queue);
  for (int i = 0; i < _numRows;i++) {
    pinMode(_rowPin[i], OUTPUT);
    digitalWrite(_rowPin[i], HIGH);
  }

  for (int i = 0; i < _numColumns; i++) {
    pinMode(_columnPin[i], INPUT_PULLUP);
  }
}

void GPIOKeyMatrix::scanMatrix() {
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

boolean GPIOKeyMatrix::update() {

  keyswitch_t count = 0;

  clearKeyChanges();

  millis_t now = Uptime::millis();

  //console.debugf("checking now: %d.%03d _nextScan: %d.%03d\n", (int)(now/1000),(int)(now%1000), (int)(_nextScan/1000),(int)(_nextScan%1000));

  if (now >= _nextScan) {
    //console.debugln("scanning");
    scanMatrix();
    keyswitch_t total = _numRows * _numColumns;

    for (keyswitch_t i = 0; i < total; i++) {
      if ((i != NO_KEY) && ((_changedKeys[i/_numRows] >> (i%_numRows)) & 0x01)) {
        count++;
        bool d = switchIsDown(i);
        if (keyDebug) console.debugf("GPIO key %d %s\n", i, d ? "down" : "up");
        _queue->addEvent(this, i,getCode(i),now, d);
      }
    }
    if (count) {
      _nextScan = now + _debounceInterval;
      if (keyDebug) console.debugf("update found %d key events, next scan: %d.%03d\n",count, (int)(_nextScan/1000),(int)(_nextScan%1000));
    } else {
      _nextScan = now + _minScanInterval;
    }
  }
  return count != 0;
}

void GPIOKeyMatrix::clearKeyChanges() {
  // save the last state
  for (int i = 0; i < _numColumns; i++) {
    _changedKeys[i] = 0;
  }
}

