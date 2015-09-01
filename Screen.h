#ifndef _SCREEN_
#define _SCREEN_

// get some shared typedefs, like Direction
#include "Types.h"

#include <ILI9341_t3.h>

typedef uint16_t color_t;
typedef int16_t coord_t;

class Screen : public ILI9341_t3 {
  public:


    const color_t black = ILI9341_BLACK;
    const color_t white = ILI9341_WHITE;

    const color_t red = ILI9341_RED;
    const color_t green = ILI9341_GREEN;
    const color_t blue = ILI9341_BLUE;
    const color_t yellow = ILI9341_YELLOW;
    const color_t orange = ILI9341_ORANGE;

    const color_t darkred =   0x8000; // .5, 0, 0
    const color_t darkgreen = 0x0400; // 0, .5, .0
    const color_t darkblue =  0x0010; // 0, 0, .5
    const color_t darkyellow =  0x8400; // .5, .5., 0

    const color_t darkerred =   0x4000; // .25, 0, 0
    const color_t darkergreen = 0x0200; // 0, .25, .0
    const color_t darkerblue =  0x0008; // 0, 0, .s5

    const color_t grey = 0x4208; // .25, .25, .25

    const color_t bluegreen = 0x0410; // 0,  .5, .5

    const uint8_t fontHeight = 8;

    Screen(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9341_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO) {
    };

    void drawText(const char* text);
    void drawTextF(const char* format, ...);
    int measureTextH(const char* text);
    int measureTextV(const char* text);

    void pushFill(Direction dir, color_t color);

    color_t darken(color_t c);
    color_t brighten(color_t c);
    color_t mix(color_t c1, color_t c2);
    uint8_t luminance(color_t);

protected:
};

#endif
