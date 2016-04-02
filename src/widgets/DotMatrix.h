#ifndef _DotMatrix_
#define _DotMatrix_

#include "Screen.h"
#include "Widget.h"

class DotMatrix : public Widget {

  public:
    DotMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh);
    virtual ~DotMatrix() { if (dots) { delete dots; } }

    virtual void draw(); // redraws every dot, can be slow
    virtual void clear();  // clears out dots (to bgColor) and erases screen

    virtual int getDotsWide() { return dots_wide; }
    virtual int getDotsHigh() { return dots_high; }

    virtual void updateDot(int x, int y);
    inline void setDot(int x, int y, color_t c) { dots[x*dots_high+y] = c; }
    inline color_t getDot(int x, int y) { return dots[x*dots_high+y]; }
    inline bool valid(int x, int y) { return !((x < 0) || (y < 0) || (x >= getDotsWide()) || (y >= getDotsHigh())); }

    virtual bool hit(coord_t x, coord_t y, int* hitx = nullptr, int* hity = nullptr);
    void setBGColor(color_t bg) { bgColor = bg; };
    void setBorder(coord_t b) { border = b; }

  protected:
    int dot_fill = 80; // dot size in matrix cell, percent
    int dots_wide = 0;
    int dots_high = 0;
    int dotspacing_w;
    int dotspacing_h;
    int r;
    color_t* dots = nullptr;
    color_t bgColor = screen.black;
    coord_t border = 0;
};

class SquareMatrix : public DotMatrix {
  public:
    SquareMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh) : DotMatrix(x, y, w, h, dotsWide, dotsHigh) {};
    virtual void updateDot(int x, int y);
};

class HexDotMatrix : public DotMatrix {
  public:
    HexDotMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh) : DotMatrix(x, y, w, h, dotsWide, dotsHigh) {};
    void setStaggerV(bool staggerVertically) {staggerV = staggerVertically; }
    virtual void updateDot(int x, int y);
  protected:
    bool staggerV = false;
};

class TriangleMatrix : public HexDotMatrix {
  public:
    TriangleMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dotsWide, int dotsHigh) : HexDotMatrix(x, y, w, h, dotsWide, dotsHigh) {};
    virtual void updateDot(int x, int y);
};

#endif

