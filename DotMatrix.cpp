#include "DotMatrix.h"


void DotMatrix::updateDot(int x, int y) {
  int center_x = x * dotspacing_w + dotspacing_w/2;
  int center_y = y * dotspacing_h + dotspacing_h/2;
  screen.fillCircle(screen.clipLeft()+center_x, screen.clipTop()+center_y, r, getDot(x,y));
}

void DotMatrix::redraw() {
  for (int x = 0; x < dots_wide; x++) {
    for (int y = 0; y < dots_high; y++) {
      updateDot(x,y);
    }
  }
}

void DotMatrix::init(int w, int h, color_t* mem) {
    dots_wide = w;
    dots_high = h;

    dotspacing_w = screen.clipWidth() / dots_wide;
    dotspacing_h = screen.clipHeight() / dots_high;

    r = min((screen.clipWidth() * 8 / dots_wide) / 10 / 2, (screen.clipHeight() * 8 / dots_high) / 10 / 2);  // dots are 80% of space
    dots = mem;
    memset(dots, 0, dots_wide*dots_high*sizeof(color_t));
}


void DotMatrix::clear(void) {
    memset(dots, 0, dots_wide*dots_high*sizeof(color_t));
    screen.fillScreen(bgColor);
}

bool DotMatrix::hit(coord_t x, coord_t y, int* hitx, int* hity) {
  if (x < screen.clipRight() &&
      y < screen.clipBottom() &&
      x >= screen.clipLeft() &&
      y >= screen.clipTop()) {
    if (hitx) {
      *hitx= x/dotspacing_w;
    }
    if (hity) {
      *hity = y/dotspacing_h;
    }
    return true;
  } else {
    return false;
  }
}
// todo: add correct HexDotMatrix::hit()

void HexDotMatrix::updateDot(int x, int y) {

  int dotspacing_x = screen.clipWidth() / dots_wide;
  int dotspacing_y = ((long)dotspacing_x * 866L) / 1000L;  // .8660 is sqrt(3)/2

  int r = (dotspacing_x * 8) / 10 / 2;  // dots are 80% of space
  int center_x = x * dotspacing_x + dotspacing_x/2 + (y%2 ? dotspacing_x / 2 : 0);
  int center_y = y * dotspacing_y + dotspacing_y/2;

  // don't draw the ones that are pushed too far right
  if (center_x + r < screen.clipRight() && center_y + r < screen.clipBottom()) {
    screen.fillCircle(screen.clipLeft() + center_x, screen.clipTop() + center_y, r, getDot(x,y));
  }
}




