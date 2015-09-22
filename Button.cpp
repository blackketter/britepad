#include "Button.h"
#include "Debug.h"

void Button::init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight, const char* title, uint8_t* icon) {
  xpos = x;
  ypos = y;
  width = w;
  height = h;
  colored = color;
  highlighted = highlight;
  titleStr = title;
  iconData = icon;
};

bool Button::down(void) {
  if (pad.down(SCREEN_PAD) && hit(pad.x(), pad.y())) {
    return true;
  } else {
    return false;
  }
}

void Button::draw(void) {

  coord_t r = min(width/2, height/2);

  screen.fillCircle( xpos+width/2, ypos+height/2, r, highlighted ? screen.mix(colored, screen.black) : colored);
  screen.setFont(Arial_9_Bold);
  screen.setTextColor(screen.black);
  screen.setCursor( xpos+height/2 - screen.measureTextH(titleStr) / 2, ypos+width/2 - screen.measureTextV(titleStr) / 2);
  screen.drawText(titleStr);
};
