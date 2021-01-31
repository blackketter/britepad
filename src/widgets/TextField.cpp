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

  if (_bColor != _fColor) {
    screen.fillScreen(_bColor);
  }

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
  }

  screen.pushClipRect(&r);
}

bool TextField::key(KeyEvent* k) {
  if (k && k->pressed()) {
    console.debugln("got key");
    char c = keyEvents->getKeyChar(k->code());
    if (isprint(c) && _text.length()<_maxLength) { // printable character
      console.debugf("adding char %c\n",c);
      _text.concat(c);
      draw();
      return true;
    } if (c == 0x08) { // backspace
      console.debugf("backspace\n");
      _text.remove(_text.length()-1);
      draw(nullptr);
      draw();
      return true;
    }
  }
  return false;
}
