#ifndef _DotMatrix_
#define _DotMatrix_

#include "Screen.h"

class DotMatrix {

  public:
    virtual void init(int dotsWide, int dotsHigh, void* mem);
    virtual void redraw(void);
    virtual void clear(void);

    int getDotsWide() { return dots_wide; }
    int getDotsHigh() { return dots_high; }
    virtual void updateDot(int x, int y);
    inline void setDot(int x, int y, color_t c) { dots[x*dots_high+y] = c; }
    inline color_t getDot(int x, int y) { return dots[x*dots_high+y]; }


  protected:
    int dots_wide = 0;
    int dots_high = 0;
    int dotspacing_w;
    int dotspacing_h;
    int r;
    color_t* dots = 0;


};

class HexDotMatrix : public DotMatrix {
  public:
    virtual void updateDot(int x, int y);
};


#endif

