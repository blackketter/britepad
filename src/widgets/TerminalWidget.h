#ifndef _TerminalWidget_
#define _TerminalWidget_
#include "widgets/Widget.h"
#include "KeyMatrix.h"

class TerminalWidget : public Widget, public Stream {
  public:
    TerminalWidget(coord_t x, coord_t y, coord_t w, coord_t h, uint16_t scrollbackLines = 0);
    ~TerminalWidget();

// Widget methods
    virtual void draw();
    virtual void run();
    virtual bool key(Event* k);

// Stream methods
    virtual int available();
    virtual int read();
    virtual int peek();
    virtual void flush();
    virtual size_t write(uint8_t b);

    void clearScreen();

  private:
    char* _buffer = nullptr;

    color_t _bgcolor = screen.black;
    color_t _fgcolor = screen.white;
    color_t _historyColor = screen.lightgrey;
    color_t _cursorColor = screen.grey;

    uint8_t _columns; // visible columns
    uint8_t _rows;    // visible rows
    uint32_t _cursor; // where the next character will be drawn

    uint32_t _displayOffset; // offset in buffer for first character displayed
    uint32_t _totalRows; // total rows, including scrollback
    uint32_t _totalChars; // total characters (i.e. _totalRows * _columns)
    uint32_t _charsPerScreen;

    // 5x7 font
    uint8_t _charSize = 1;
    coord_t _charWidth = 6 * _charSize;
    coord_t _charHeight = 8 * _charSize;
    const ILI9341_t3_font_t* _font;

    char _lastKey = 0;
    uint8_t _lastChar = 0;
    bool _toUpdate = false;
};
#endif
