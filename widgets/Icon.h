#ifndef _Icon_
#define _Icon_

#include "Screen.h"
#include "widgets/Icons.h"
// icondata format is:
// 1 byte of width
// 1 byte of height
// rest of bytes - packed bitmap of width*height/8 bytes (rounded up to fit all the bits)

typedef const uint8_t* icon_t;

class Icon {

public:
  Icon(icon_t icondata) { data = icondata; };
  Icon() {};

  void setData(icon_t icondata) { data = icondata; };
  icon_t getData() { return data; }
  coord_t width() { return data ? (coord_t)data[0] : 0; }
  coord_t height() { return data ? (coord_t)data[1] : 0; }
  void draw(coord_t x, coord_t y, color_t color = screen.white) { if (data) screen.drawBitmap(x, y, &data[2], width(), height(), color); }
  void draw(coord_t x, coord_t y, color_t fgcolor, color_t bgcolor) { if (data) screen.drawBitmap(x, y, &data[2], width(), height(), fgcolor, bgcolor); }

private:
  icon_t data = nullptr;
};

#endif
