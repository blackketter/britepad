#include "Screen.h"
#include "Utilities.h"

void Screen::setBacklight(uint8_t brightness) {
  backlightBrightness = brightness;
#if TEENSY
  analogWriteFrequency(BACKLIGHT_PIN, BACKLIGHT_FREQUENCY);
#endif
  analogWrite(BACKLIGHT_PIN, brightness);
}

uint8_t Screen::getBacklight() {
  return backlightBrightness;
}


void Screen::polarToRect(uint16_t theta, coord_t r, coord_t& x, coord_t& y) {
  coord_t center_x = screen.clipMidWidth();
  coord_t center_y = screen.clipMidHeight();

  x = center_x + ((int32_t)r*sin16(theta))/32768L;
  y = center_y - ((int32_t)r*cos16(theta))/32768L;
}

// todo - make ends of lines square (or round)
void Screen::drawWideLine(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t width, color_t color) {
  if (width == 1) {
    drawLine(x0, y0, x1, y1, color);
  } else if (width < 1) {
    return;
  } else if (x1 == x0) {
    if (y1>y0) {
      fillRect(x0, y0, width, y1-y0+width, color);
    } else {
      fillRect(x1, y1, width, y0-y1+width, color);
    }
  } else if (y0 == y1) {
    if (x1>x0) {
      fillRect(x0, y0, x1-x0+width, width, color);
    } else {
      fillRect(x1, y1, x0-x1+width, width, color);
    }
  } else {
    coord_t dx;
    coord_t dy;
    if (abs(x0-x1) < abs(y0-y1)) {
      dx=1;
      dy=0;
    } else {
      dx = 0;
      dy = 1;
    }

    x0 -= dx*width/2;
    x1 -= dx*width/2;
    y0 -= dy*width/2;
    y1 -= dy*width/2;

    for (int i = 0; i < width; i++) {
      drawLine(x0, y0, x1, y1, color);
      x0+=dx;
      x1+=dx;
      y0+=dy;
      y1+=dy;
    }
  }
};

void Screen::drawTextF(const char* format, ...) {
  char foo[500];

  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  drawText(foo);
}

void Screen::softWrapText(String& out, const char* in) {
  const char* wrapChars = " -/";

  uint16_t curWidth = 0;
  const char* cursor = in;
  const char* curLine = cursor;
  const char* lastBreak = 0;

  while (*cursor != 0) {
    uint16_t w, h;
    measureChar(*cursor, &w, &h);
    curWidth+=w;
    if (curWidth>clipWidth()) {
      if (lastBreak == 0) {
        lastBreak = cursor;
      }
      while (curLine < lastBreak) {
        out.append(*curLine);
        curLine++;
      }
      out.append('\n');

      lastBreak = 0;
      curWidth = 0;
      cursor = curLine;
      while (*cursor == ' ') {
        cursor++;
        curLine++;
      }
    } else {
      if (strchr(wrapChars, *cursor)) {
        lastBreak = cursor;
        if (*cursor != ' ') {
          lastBreak++;
        }
      }
      cursor++;
    }
  }
  if (cursor > curLine) {
    out.append(curLine);
  }
}

// note: this does the whole screen, not just the clip area
void Screen::pushFill(direction_t dir, color_t color) {
  const coord_t stepSize = 4;
  rect_t r;
  getScreenBounds(&r);
  switch(dir) {
    case DIRECTION_UP:
      for (coord_t i = height() - stepSize; i >= 0; i -= stepSize) {
        fillRect(0, i, width(), stepSize, color);
        delay(1);
      }
      break;
    case DIRECTION_DOWN:
      for (coord_t i = 0; i < height(); i += stepSize) {
        fillRect(0, i, width(), stepSize, color);
        delay(1);
      }
      break;
    case DIRECTION_LEFT:
      pushClipRect(&r);
      for (coord_t i = 0; i < width(); i += stepSize) {
        fillRect(width()-stepSize-i, 0, stepSize, height(), color);
        screen.setScroll(i+stepSize);
        delay(1);
      }
      pushClipRect(&r);
      break;
    case DIRECTION_RIGHT:
      pushClipRect(&r);
      for (coord_t i = 0; i < width(); i+= stepSize) {
        fillRect(i, 0, stepSize, height(), color);
        screen.setScroll(width()-stepSize-i);
        delay(1);
      }
      pushClipRect(&r);
      break;
    default:
      break;
  }
}

color_t Screen::darken(color_t c) {
  uint8_t r, g, b;
  r = (c & 0xf800) >> 12;
  g = (c & 0x07e0) >> 6;
  b = (c & 0x001f) > 2;
  return r << 11 | g << 5 | b;

};

color_t Screen::brighten(color_t c, uint8_t offset) {
  uint16_t r, g, b;
  r = R8(c) + offset;
  g = G8(c) + offset;
  b = B8(c) + offset;
  r = min(r, 255);
  g = min(g, 255);
  b = min(b, 255);

  return RGBtoC16(r,g,b);
};

color_t Screen::mix(color_t c1, color_t c2) {
  uint16_t r, g, b;

  r = ((uint16_t)R8(c1) + R8(c2))/2;
  g = ((uint16_t)G8(c1) + G8(c2))/2;
  b = ((uint16_t)B8(c1) + B8(c2))/2;

  return RGBtoC16(r,g,b);
};


color_t Screen::add(color_t c1, color_t c2) {
  uint16_t r, g, b;

  r = min(255,(uint16_t)R8(c1) + R8(c2));
  g = min(255,(uint16_t)G8(c1) + G8(c2));
  b = min(255,(uint16_t)B8(c1) + B8(c2));

  return RGBtoC16(r,g,b);
};


uint8_t Screen::luminance(color_t c) {
  uint32_t r, g, b;
  r = R8(c);
  g = G8(c);
  b = B8(c);
  return sqrt16(r*r+g*g+b*b);
};
