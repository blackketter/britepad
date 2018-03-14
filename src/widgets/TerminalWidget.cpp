#include "BritepadShared.h"
#include "widgets/TerminalWidget.h"

TerminalWidget::TerminalWidget(Stream* stream, coord_t x, coord_t y, coord_t w, coord_t h) {
  _stream = stream;
  setBounds(x,y,w,h);
  _rows = h / _charHeight;
  _columns = w / _charWidth;
  _buffer = (char*)malloc(_rows*_columns);
}

void TerminalWidget::clearScreen() {
  uint32_t chars = _rows*_columns;
  for (uint32_t i = 0; i < chars; i++) {
    _buffer[i] = ' ';
  }
}

TerminalWidget::~TerminalWidget() {
  if (_buffer) {
    free(_buffer);
  }
}

void TerminalWidget::draw() {
//  screen.fillRect(_xpos, _ypos, _width, _height, screen.red);
  for (uint8_t i = 0; i < _rows; i++) {
    for (uint8_t j = 0; j < _columns; j++) {
      screen.drawChar(_xpos+j*_charWidth, _ypos+i*_charHeight, _buffer[i*_columns+j], _fgcolor, _bgcolor, 1);
    }
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
  if (_cursor >= _rows * _columns) {
    memmove ( _buffer, _buffer+_columns, _columns*(_rows-1) );
    _cursor = _columns*(_rows-1);
    memset(&_buffer[_cursor],' ',_columns);
  }

  if (b == '\r' || b == '\n') {
    if ((b == '\r' && _lastChar == '\n') || (b == '\n' && _lastChar == '\r')) {
      // cr-lf or lf-cr sequence
    } else {
      _cursor = ((_cursor / _columns) + 1) * _columns;
    }
  } else {
    _buffer[_cursor] = b;
    _cursor++;
  }
  _toUpdate = true;
  _lastChar = b;
  return 1;
}
