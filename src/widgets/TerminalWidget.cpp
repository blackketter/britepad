#include "BritepadShared.h"
#include "widgets/TerminalWidget.h"

TerminalWidget::TerminalWidget(Stream* stream, coord_t x, coord_t y, coord_t w, coord_t h, uint16_t scrollbackLines) {
  _stream = stream;
  setBounds(x,y,w,h);
  _rows = h / _charHeight;
  _columns = w / _charWidth;

  _totalRows = _rows+scrollbackLines;
  _totalChars = _totalRows*_columns;
  _charsPerScreen = _rows*_columns;

  _buffer = (char*)malloc(_totalChars);
  if (!_buffer) {
    console.debugln("Couldn't allocate TerminalWidget buffer!");
  }
  clearScreen();
}

void TerminalWidget::clearScreen() {
  _displayOffset = _totalChars - _charsPerScreen;
  _cursor = _displayOffset;

  for (uint32_t i = 0; i < _totalChars; i++) {
    _buffer[i] = ' ';
  }
  _toUpdate = true; // force a redraw
}

TerminalWidget::~TerminalWidget() {
  if (_buffer) {
    free(_buffer);
  }
}

void TerminalWidget::draw() {
  for (uint8_t i = 0; i < _rows; i++) {
    for (uint8_t j = 0; j < _columns; j++) {
      uint32_t charOffset = i*_columns+j + _displayOffset;
      bool lastScreen = charOffset > (_totalChars - _charsPerScreen);
      screen.drawChar(_xpos+j*_charWidth, _ypos+i*_charHeight, _buffer[charOffset],
        lastScreen ? _fgcolor : _historyColor,
        _bgcolor, _charSize);
    }
  }
  // draw scrollbar
  if (_totalRows > _rows) {
    uint32_t row = _displayOffset / _columns;
    coord_t x = getRight()-1;
    coord_t y0 = getTop() + getHeight() * row / _totalRows;
    coord_t y1 = getTop() + getHeight() * (row+_rows) / _totalRows;
    screen.drawLine( x, 0, x, y0, _bgcolor);
    screen.drawLine( x, y0, x, y1, _fgcolor);
    screen.drawLine( x, y1, x, _ypos+_height, _bgcolor);
  }
}

void TerminalWidget::run() {
  if (_toUpdate) {
    _toUpdate = false;
    draw();
  }
}

bool TerminalWidget::key(KeyEvent* k) {
  if (k) {
      if (k->pressed()) {
        char c = keyEvents.getKeyChar(k->code());
        if (c) {
          _lastKey = c;
          if (_lastKey) {
            _displayOffset = _totalChars-_charsPerScreen;
          }
        } else {
          switch (k->code()) {
            case (KEY_PAGE_DOWN):
              _displayOffset += _charsPerScreen;
              if (_displayOffset > (_totalChars-_charsPerScreen)) {
                _displayOffset = _totalChars-_charsPerScreen;
              }
              _toUpdate = true;
              break;

            case (KEY_PAGE_UP):
              if (_displayOffset < _charsPerScreen) {
                _displayOffset = 0;
              } else {
                _displayOffset -= _charsPerScreen;
              }
              _toUpdate = true;
              break;
            default:
              break;
          }
        }
      }
    return true;
  }
  return false;
}


int TerminalWidget::available() {
  return _lastKey ? 1 : 0;
}

int TerminalWidget::read() {
  char key = _lastKey;
  _lastKey = 0;
  return key;
}

int TerminalWidget::peek() {
  return _lastKey;
}

void TerminalWidget::flush() {
}

size_t TerminalWidget::write(uint8_t b) {
  if (_cursor >= _totalChars) {
    memmove ( _buffer, _buffer+_columns, _columns*(_totalRows-1) );
    _cursor = _columns*(_totalRows-1);
    memset(&_buffer[_cursor],' ',_columns);
  }

  if (b == '\b') {
    // backspace
    if (_cursor) { _cursor--; }
  } else if (b == '\r' || b == '\n') {
    // CR and LF - todo: should these be handled differently?
    if ((b == '\r' && _lastChar == '\n') || (b == '\n' && _lastChar == '\r')) {
      // cr-lf or lf-cr sequence
    } else {
      _cursor = ((_cursor / _columns) + 1) * _columns;
    }
  } else {
    // draw the character
    _buffer[_cursor] = b;
    _cursor++;
  }
  _toUpdate = true;
  _lastChar = b;
  return 1;
}
