#include "TextField.h"
void TextField::init(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, int8_t hAlign,int8_t vAlign, const char* t) {

  setBounds(x,y,w,h);
  font = f;
  fColor = fgColor;
  bColor = bgColor;
  alignV = vAlign;
  alignH = hAlign;
  text = t;
};


void TextField::draw() {
  draw(text);
};

void TextField::drawf(const char* format, ...) {
  char foo[500];

  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  draw(foo);
}

void TextField::draw(const char* t) {
  rect_t r;
  getBounds(r);
  screen.pushClipRect(&r);
  screen.setFont(font);
  coord_t x, y;

  // todo vertical alignment
  y = getTop();
  coord_t w = screen.measureTextWidth(t);
  coord_t h = screen.measureTextHeight(t);

  if (alignH == ALIGN_LEFT) {
    x = getLeft();
  } else {
    if (alignH == ALIGN_CENTER) {
      x = getMidWidth()-w/2;
    } else {
      x = getRight()-w;
    }
  }

  screen.setCursor(x,y);

  screen.setTextColor(fColor,bColor);
  screen.drawText(t);

  if (fColor != bColor) {
    screen.fillRect(getLeft(),getTop(),x - getLeft(), h, bColor);
    screen.fillRect(x+w, getTop(), getRight()-(x+w), h, bColor);
  }

  screen.pushClipRect(&r);
}

