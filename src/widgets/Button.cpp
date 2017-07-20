#include "widgets/Button.h"

void Button::init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, icon_t iconData, widgetid_t id, keycode_t key) {
  setBounds(x,y,w,h);

  setColor(color);
  highlighted = highlight;
  titleStr = title;
  titleFont = f;
  titleCol = titleColor;
  _visible = true;
  icon.setData(iconData);
  setID(id);
  setKey(key);
};

void Button::init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, icon_t iconData) {
  _xpos = x - r;
  _ypos = y - r;
  _width = r*2;
  _height = r*2;
  setColor(color);
  highlighted = highlight;
  titleStr = title;
  titleFont = f;
  titleCol = titleColor;
  _visible = true;
  icon.setData(iconData);
}

bool Button::pressed() {
  track();
  if (pad.pressed(SCREEN_PAD) && hit(pad.x(), pad.y())) {
    sound.click();
    return true;
  } else {
    return false;
  }
}

bool Button::held() {
  track();
  if (getHighlighted() && (pad.time() - highlightedTime > holdTime)) {
    return true;
  } else {
    return false;
  }
}

bool Button::released() {
  track();
  if (pad.released(SCREEN_PAD) && hit(pad.x(), pad.y())) {
    sound.click();
    return true;
  } else {
    return false;
  }
}

void Button::track() {
  bool h = hit(pad.x(), pad.y());
  bool t = pad.touched(SCREEN_PAD);
  bool r = pad.released(SCREEN_PAD);

  if (highlighted) {
    if (r || (t && !h)) {
      setHighlighted(false);
    }
  } else {
    if (t && h) {
      setHighlighted(true);
    }
  }
}

color_t Button::fillColor() {
  return highlighted ? screen.mix(getColor(), screen.black) : getColor();
}

void Button::drawbg() {
  screen.fillRect(_xpos, _ypos,_width, _height, fillColor());
  if (selected) {
    screen.drawRect(_xpos, _ypos,_width, _height, screen.white);
    screen.drawRect(_xpos+1, _ypos+1,_width-1, _height-1, screen.white);
  }
}

void Button::drawIcon() {
//  uint8_t hash[] = {8,8,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55};
//  icon.setData(hash);
  Icon i = getIcon();
  i.draw(_xpos+_width/2-i.width()/2, _ypos+_height/2-i.height()/2, getTitleColor());
}

void Button::draw() {
  if (_visible) {
    drawbg();
    drawIcon();
    drawTitle();
  }
}

void Button::drawTitle() {

  const char* drawStr;
  char onechar[2];

  if (!getTitle()) {
    return;
  }

  if ((uint32_t)getTitle() > 0xff) {
    drawStr = getTitle();
  } else {
    onechar[0] = (uint32_t)getTitle();
    onechar[1] = 0;
    drawStr = onechar;
  }

  screen.setFont(getTitleFont());
  screen.setTextColor(getTitleColor());
  screen.setCursor( _xpos+_width/2, _ypos+_height/2);
  screen.setTextAlign(alignment_t(ALIGN_HCENTER|ALIGN_VCENTER));

  rect_t r = { (coord_t)(_xpos+_frameInset), (coord_t)(_ypos+_frameInset), (coord_t)(_width-_frameInset*2), (coord_t)(_height-_frameInset*2)};
  screen.pushClipRect(&r);
  if (_wrapTitle) {
    String wrapped;
    screen.softWrapText(wrapped,drawStr);
    screen.drawString(wrapped);
  } else {
    screen.drawText(drawStr);
  }

  screen.pushClipRect(&r);

  screen.setTextAlign();
}

void RoundButton::drawbg() {
  coord_t r = (min(_width, _height)-1)/2;  // round down radius from diameter (fillCircle circles have diameter 2r+1)
  screen.fillCircle( _xpos+_width/2, _ypos+_height/2, r, fillColor());
  if (selected) {
    screen.drawCircle( _xpos+_width/2, _ypos+_height/2, r, screen.white);
    screen.drawCircle( _xpos+_width/2, _ypos+_height/2, r-1, screen.white);
  }
};

void RoundedButton::drawbg() {
  screen.fillRoundRect(_xpos, _ypos, _width, _height, radius, fillColor());
  if (selected) {
    screen.drawRoundRect(_xpos, _ypos, _width, _height, radius, screen.white);
    screen.drawRoundRect(_xpos+1, _ypos+1, _width-1, _height-1, radius-1, screen.white);
  }
}

