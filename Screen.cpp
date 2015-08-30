#include <ILI9341_t3.h>
#include "Screen.h"
#include "Debug.h"

void Screen::drawText(const char* text) {

    int origx = cursor_x;

    int i = 0;
    while (text[i] != 0) {
      write(text[i]);

      // don't wrap to the left, wrap to the original spot
      if (wrap && text[i] == '\n') {
        cursor_x = origx;
      }
      i++;
    }
}

int Screen::measureTextH(const char* text) {
  int maxH = 0;
  int currH = 0;
  for (const char* i = text; *i != 0; i++) {
    if (*i == '\n') {
      if (currH > maxH)
        maxH = currH;
      currH = 0;
    } else {
      currH += 6 * textsize;
    }
  }

  return maxH > currH ? maxH : currH;
}

int Screen::measureTextV(const char* text) {
  int lines = 1;
  for (const char* i = text; *i != 0; i++) {
    if (*i == '\n') {
      lines++;
    }
  }
  return (lines * 8 * textsize);
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
  const int stepSize = 4;

  switch(dir) {
    case DIRECTION_UP:
      for (int i = height() - stepSize; i >= 0; i -= stepSize) {
        fillRect(0, i, width(), stepSize, color);
        delay(1);
      }
      break;
    case DIRECTION_DOWN:
      for (int i = 0; i < height(); i += stepSize) {
        fillRect(0, i, width(), stepSize, color);
        delay(1);
      }
      break;
    case DIRECTION_LEFT:
      for (int i = 0; i < width(); i += stepSize) {
        fillRect(i, 0, stepSize, height(), color);
        delay(1);
      }
      break;
    case DIRECTION_RIGHT:
      for (int i = width() - stepSize; i >= 0; i -= stepSize) {
        fillRect(i, 0, stepSize, height(), color);
        delay(1);
      }
      break;
    default:
      break;
  }
}
