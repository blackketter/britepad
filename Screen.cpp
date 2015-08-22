#include <ILI9341_t3.h>
#include "Screen.h"

#define TFT_CS 10
#define TFT_DC 9

void Screen::drawText(const char* text, int x, int y) {
    if (x < 0 || y < 0) {
      // location not set, continue where we left off
    } else {
      setCursor(x,y);
    }

    int i = 0;
    while (text[i] != 0) {
      write(text[i++]);
    }
}

void Screen::drawTextF(const char* format, ...) {
  char foo[80];

  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  drawText(foo);
}
