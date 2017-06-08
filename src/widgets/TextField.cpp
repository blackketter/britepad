#include "TextField.h"
void TextField::init(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, alignment_t a) {

  setBounds(x,y,w,h);
  _font = f;
  _fColor = fgColor;
  _bColor = bgColor;
  _align = a;
};

void TextField::draw(const char* t) {
  rect_t r;
  getBounds(r);
  screen.pushClipRect(&r);

  if (t) {

    coord_t x, y;

    if (_align | ALIGN_HCENTER) {
      x = getMidWidth();
    } else if (_align | ALIGN_RIGHT) {
      x = getRight();
    } else {
      x = getLeft();
    }

    if (_align | ALIGN_VCENTER) {
      y = getMidHeight();
    } else if (_align | ALIGN_BOTTOM) {
      y = getBottom();
    } else {
      y = getTop();
    }

    screen.setFont(_font);
    screen.setTextAlign(_align);

    screen.setCursor(x,y);

    screen.setTextColor(_fColor,_bColor);
    screen.drawText(t);
    screen.setTextAlign();
  } else {
    // fill the whole clipped area
    screen.fillScreen(_bColor);
  }

  screen.pushClipRect(&r);
}

bool TextField::key(KeyEvent* k) {

}