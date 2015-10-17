#include <ILI9341_t3.h>
#include <FastLED.h>
#include "Screen.h"
#include "Debug.h"

void Screen::backlight(uint8_t brightness) {
  analogWriteFrequency(BACKLIGHT_PIN, BACKLIGHT_FREQUENCY);
  analogWrite(BACKLIGHT_PIN, brightness);
}

void Screen::drawText(const char* text) {

    coord_t origx = cursor_x;

    int i = 0;
    while (text[i] != 0) {
      write(text[i]);

      // don't wrap to the left, wrap to the original spot
      if (wrap && text[i] == '\n') {
        cursor_x = origx;
        cursor_y += fontHeight() + fontLineSpacing();
      }
      i++;
    }
}

coord_t Screen::measureTextH(const char* text) {
  coord_t maxH = 0;
  coord_t currH = 0;
  for (const char* i = text; *i != 0; i++) {
    if (*i == '\n') {
      if (currH > maxH)
        maxH = currH;
      currH = 0;
    } else {
      uint16_t h, w;
      measureChar(*i, &w, &h);
      currH += w;
    }
  }
  coord_t h = maxH > currH ? maxH : currH;
  return h;
}

coord_t Screen::measureTextV(const char* text) {
  int lines = 1;
  for (const char* i = text; *i != 0; i++) {
    if (*i == '\n') {
      lines++;
    }
  }
  return (lines * fontHeight() + (lines-1)*fontLineSpacing());
}

void Screen::drawTextF(const char* format, ...) {
  char foo[80];

  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  drawText(foo);
}

void Screen::pushFill(Direction dir, color_t color) {
  const coord_t stepSize = 4;

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
      for (coord_t i = 0; i < width(); i += stepSize) {
        fillRect(i, 0, stepSize, height(), color);
        delay(1);
      }
      break;
    case DIRECTION_RIGHT:
      for (coord_t i = width() - stepSize; i >= 0; i -= stepSize) {
        fillRect(i, 0, stepSize, height(), color);
        delay(1);
      }
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
  r = R8(c) + 64;
  g = G8(c) + 64;
  b = B8(c) + 64;
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
