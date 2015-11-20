#include <ILI9341_t3.h>
#include <FastLED.h>
#include "Screen.h"
#include "Debug.h"

void Screen::setBacklight(uint8_t brightness) {
  backlightBrightness = brightness;
  analogWriteFrequency(BACKLIGHT_PIN, BACKLIGHT_FREQUENCY);
  analogWrite(BACKLIGHT_PIN, brightness);
}

uint8_t Screen::getBacklight() {
  return backlightBrightness;
}

// todo - make ends of lines square (or round)
void Screen::drawWideLine(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t width, color_t color) {
  if (width == 1) {
    ILI9341_t3::drawLine(x0, y0, x1, y1, color);
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
      ILI9341_t3::drawLine(x0, y0, x1, y1, color);
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

  return ((r & 0xf8) << 8) | ((g & 0xfc ) << 3) | (b >> 3);
};

color_t Screen::mix(color_t c1, color_t c2) {
  uint16_t r, g, b;

  r = (R8(c1) + R8(c2))/2;
  g = (G8(c1) + G8(c2))/2;
  b = (B8(c1) + B8(c2))/2;

  return ((r & 0xf8) << 8) | ((g & 0xfc ) << 3) | (b >> 3);
};


uint8_t Screen::luminance(color_t c) {
  uint16_t r, g, b;
  r = R8(c);
  g = G8(c);
  b = B8(c);
  return sqrt16(r*r+g*g+b*b);
};
