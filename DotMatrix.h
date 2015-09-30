#ifndef _DotMatrix_
#define _DotMatrix_

#include "Screen.h"

class DotMatrix {

  public:
    virtual void init(int dotsWide, int dotsHigh, void* mem);
    virtual void redraw(void); // redraws every dot, can be slow
    virtual void clear(void);  // clears out dots (to bgColor) and erases screen

    int getDotsWide() { return dots_wide; }
    int getDotsHigh() { return dots_high; }
    virtual void updateDot(int x, int y);
    inline void setDot(int x, int y, color_t c) { dots[x*dots_high+y] = c; }
    inline color_t getDot(int x, int y) { return dots[x*dots_high+y]; }

    virtual bool hit(coord_t x, coord_t y, int* hitx = nil, int* hity = nil);
    void setBGColor(color_t bg) { bgColor = bg; };

  protected:
    int dots_wide = 0;
    int dots_high = 0;
    int dotspacing_w;
    int dotspacing_h;
    int r;
    color_t* dots = 0;
    color_t bgColor = screen.black;

};

class HexDotMatrix : public DotMatrix {
  public:
    virtual void updateDot(int x, int y);
};


#endif

