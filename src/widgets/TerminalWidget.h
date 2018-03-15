#ifndef _TerminalWidget_
#define _TerminalWidget_
#include "widgets/Widget.h"
#include "KeyMatrix.h"

class TerminalWidget : public Widget, public Stream {
  public:
    TerminalWidget(Stream* stream, coord_t x, coord_t y, coord_t w, coord_t h, uint16_t scrollbackLines = 0);
    ~TerminalWidget();

// Widget methods
    virtual void draw();
    virtual void run();
    virtual bool key(KeyEvent* k);

// Stream methods
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
    virtual size_t write(uint8_t b);

    void clearScreen();

  private:
    Stream* _stream = nullptr;
    char* _buffer = nullptr;

    color_t _bgcolor = screen.black;
    color_t _fgcolor = screen.white;
    color_t _historyColor = screen.lightgrey;

    uint8_t _columns; // visible columns
    uint8_t _rows;    // visible rows
    uint32_t _cursor; // where the next character will be drawn

    uint32_t _displayOffset; // offset in buffer for first character displayed
    uint32_t _totalRows; // total rows, including scrollback
    uint32_t _totalChars; // total characters (i.e. _totalRows * _columns)
    uint32_t _charsPerScreen;

    // 5x7 font
    const uint8_t _charSize = 1;
    const coord_t _charWidth = 6 * _charSize;
    const coord_t _charHeight = 8 * _charSize;

    char _lastKey = 0;
    uint8_t _lastChar = 0;
    bool _toUpdate = false;
};
#endif