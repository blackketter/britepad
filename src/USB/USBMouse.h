#ifndef _USBMouse_
#define _USBMouse_

#include "BritepadShared.h"

class USBMouse {
  public:
    void begin() {}
    void end() {}
    void run();

    coord_t x() { return _x; }
    coord_t y() { return _y; }
    void setLoc(coord_t x, coord_t y) { _x = x; _y = y; }
    millis_t lastMove() { return _lastmove; }

    coord_t getScroll() { return _wheel; }
    void setScroll(coord_t scroll) { _wheel = scroll; }

    uint8_t buttons() { return _buttons; }

    void setBounds(coord_t x, coord_t y, coord_t w, coord_t h);
    void checkBounds();

  private:
    coord_t _x;
    coord_t _y;
    coord_t _dx;
    coord_t _dy;
    int8_t _dwheel;
    coord_t _wheel;
    uint8_t _buttons;
    rect_t _bounds;
    millis_t _lastmove;
};

extern USBMouse usbMouse;

#endif
