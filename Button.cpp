#include "Button.h"
#include "Debug.h"

void Button::init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, const uint8_t* iconData) {
  xpos = x;
  ypos = y;
  width = w;
  height = h;
  colored = color;
  highlighted = highlight;
  titleStr = title;
  titleFont = f;
  titleCol = titleColor;
  visible = true;
  icon.setData(iconData);
};

bool Button::down(void) {
  track();
  if (pad.down(SCREEN_PAD) && hit(pad.x(), pad.y())) {
    sound.click();
    return true;
  } else {
    return false;
  }
}

bool Button::hold(void) {
  track();
  if (getHighlighted() && (pad.time() - highlightedTime > holdTime)) {
    return true;
  } else {
    return false;
  }
}

bool Button::up(void) {
  track();
  if (pad.up(SCREEN_PAD) && hit(pad.x(), pad.y())) {
    sound.click();
    return true;
  } else {
    return false;
  }
}

void Button::track(void) {
  bool h = hit(pad.x(), pad.y());

  if (highlighted) {
    if (pad.up(SCREEN_PAD) || (pad.touched(SCREEN_PAD) && !h)) {
      setHighlighted(false);
    }
  } else {
    if (pad.touched(SCREEN_PAD) && h) {
      setHighlighted(true);
    }
  }
}

color_t Button::fillColor(void) {
  return highlighted ? screen.mix(colored, screen.black) : colored;
}

void Button::drawbg(void) {
  screen.fillRect(xpos, ypos,width, height, fillColor());
}

void Button::drawIcon(void) {
//  uint8_t hash[] = {8,8,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};
//  icon.setData(hash);
  if (icon.getData()) {
    icon.draw(xpos+width/2-icon.width()/2, ypos+height/2-icon.height()/2, titleCol);
  }
}

void Button::draw(void) {
  if (visible) {
    drawbg();
    drawIcon();
    drawTitle();
  }
}

void Button::drawTitle(void) {

  const char* drawStr;
  char onechar[2];

  if (!titleStr) {
    return;
  }

  if ((uint32_t)titleStr > 0xff) {
    drawStr = titleStr;
  } else {
    onechar[0] = (uint32_t)titleStr;
    onechar[1] = 0;
    drawStr = onechar;
  }

  screen.setFont(titleFont);
  screen.setTextColor(titleCol);
  screen.setCursor( xpos+width/2 - screen.measureTextWidth(drawStr) / 2, ypos+height/2 - screen.measureTextHeight(drawStr) / 2);
  screen.drawText(drawStr);
}

void Button::init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, const uint8_t* iconData) {
  xpos = x - r;
  ypos = y - r;
  width = r*2;
  height = r*2;
  colored = color;
  highlighted = highlight;
  titleStr = title;
  titleFont = f;
  titleCol = titleColor;
  visible = true;
  icon.setData(iconData);
}

void RoundButton::drawbg(void) {
  coord_t r = min(width/2, height/2);
  screen.fillCircle( xpos+width/2, ypos+height/2, r, fillColor());
};

