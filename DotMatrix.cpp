#include "DotMatrix.h"
#include "Debug.h"

DotMatrix::DotMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int dots_w, int dots_h) {
  dots_wide = dots_w;
  dots_high = dots_h;

  dotspacing_w = w / dots_wide;
  dotspacing_h = h / dots_high;

//  setBounds(x,y,w,h);
  // since dots are evenly spaced and the given w & h may not be even divsors, then we need to shrink to fit the multiples
  setBounds(x,y,dotspacing_w*dots_w,dotspacing_h*dots_h);

  r = min((w * dot_fill / dots_wide) / 100 / 2, (h * dot_fill / dots_high) / 100 / 2);  // dots are 80% of space
  dots = new color_t[dots_wide*dots_high];
  memset(dots, 0, dots_wide*dots_high*sizeof(color_t));
}

void DotMatrix::updateDot(int x, int y) {
  int center_x = x * dotspacing_w + dotspacing_w/2;
  int center_y = y * dotspacing_h + dotspacing_h/2;
  screen.fillCircle(getLeft()+center_x, getTop()+center_y, r, getDot(x,y));
}

void DotMatrix::draw() {
  for (int x = 0; x < dots_wide; x++) {
    for (int y = 0; y < dots_high; y++) {
      updateDot(x,y);
    }
  }
}

void DotMatrix::clear() {
    memset(dots, 0, dots_wide*dots_high*sizeof(color_t));
    screen.fillScreen(bgColor);
}

bool DotMatrix::hit(coord_t x, coord_t y, int* hitx, int* hity) {
  if (x < getRight() &&
      y < getBottom() &&
      x >= getLeft() &&
      y >= getTop()) {
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

void SquareMatrix::updateDot(int x, int y) {
  screen.fillRect(getLeft()+x*dotspacing_w,getTop()+y*dotspacing_h,dotspacing_w,dotspacing_h,getDot(x,y));
}

// todo: add correct HexDotMatrix::hit()
// todo: create a HexMatrix that is not dotty

void HexDotMatrix::updateDot(int x, int y) {

  int dotspacing_x = getWidth() / dots_wide;
  int dotspacing_y = ((long)dotspacing_x * 866L) / 1000L;  // .8660 is sqrt(3)/2

  int r = (dotspacing_x * dot_fill) / 100 / 2;  // dots are 80% of space
  int center_x = x * dotspacing_x + dotspacing_x/2 + (y%2 ? dotspacing_x / 2 : 0);
  int center_y = y * dotspacing_y + dotspacing_y/2;

  // don't draw the ones that are pushed too far right
  if (center_x + r < getRight() && center_y + r < getBottom()) {
    screen.fillCircle(getLeft() + center_x, getTop() + center_y, r, getDot(x,y));
  }
}




