#ifndef _TerminalWidget_
#define _TerminalWidget_
#include "widgets/Widget.h"
#include "KeyMatrix.h"

class TerminalWidget : public Widget, public Stream {
  public:
    TerminalWidget(Stream* stream, coord_t x, coord_t y, coord_t w, coord_t h);
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

    uint8_t _columns;
    uint8_t _rows;
    uint16_t _cursor = 0;

    // 5x7 font
    const coord_t _charWidth = 6;
    const coord_t _charHeight = 8;

    char _lastKey = 0;
    uint8_t _lastChar = 0;
    bool _toUpdate = false;
};
#endif