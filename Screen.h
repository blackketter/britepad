#ifndef _SCREEN_
#define _SCREEN_

// get some shared typedefs, like Direction
#include "Types.h"

#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <font_ArialBold.h>

#define R8(x) (((x)&0xf800)>>8)
#define G8(x) (((x)&0x07e0)>>3)
#define B8(x) (((x)&0x001f)<<3)

typedef uint16_t color_t;
typedef int16_t coord_t;

typedef struct point_t {
  coord_t x;
  coord_t y;
} point_t;

typedef ILI9341_t3_font_t font_t;

class Screen : public ILI9341_t3 {
  public:


    const color_t black = ILI9341_BLACK;
    const color_t white = ILI9341_WHITE;

    const color_t red = ILI9341_RED;
    const color_t green = ILI9341_GREEN;
    const color_t blue = ILI9341_BLUE;
    const color_t yellow = ILI9341_YELLOW;
    const color_t orange = ILI9341_ORANGE;
    const color_t cyan = 0x07ff; // 0, 1, 1

    const color_t grey = 0x4208; // .25, .25, .25
    const color_t lightgrey = 0x8410;

    const color_t darkred =   0x8000; // .5, 0, 0
    const color_t darkgreen = 0x0400; // 0, .5, .0
    const color_t darkblue =  0x0010; // 0, 0, .5
    const color_t darkyellow =  0x8400; // .5, .5., 0

    const color_t darkerred =   0x4000; // .25, 0, 0
    const color_t darkergreen = 0x0200; // 0, .25, .0
    const color_t darkerblue =  0x0008; // 0, 0, .25
    const color_t darkergrey = 0x2104; // 0.125, .125, .125
    const color_t darkeryellow = 0x2100; // 0.125, .125, 0

    const color_t bluegreen = 0x0410; // 0,  .5, .5

    const uint8_t maxbrightness = 255;

    const coord_t offscreen = -100;

    Screen(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9341_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO) {
      pinMode(21, OUTPUT);
      digitalWrite(21, LOW);   // set the backlight off
    };

    void backlight(uint8_t brightness);

    void drawText(const char* text);
    void drawTextF(const char* format, ...);
    coord_t measureTextH(const char* text);
    coord_t measureTextV(const char* text);

    void pushFill(Direction dir, color_t color);

    color_t darken(color_t c);
    color_t brighten(color_t c, uint8_t offset = 64);
    color_t mix(color_t c1, color_t c2);
    uint8_t luminance(color_t);

    inline coord_t clipTop(void) { return _clipy1; };
    inline coord_t clipLeft(void) { return _clipx1; };
    inline coord_t clipHeight(void) { return _clipy2 - _clipy1; };
    inline coord_t clipWidth(void) { return _clipx2 - _clipx1; };
    inline coord_t clipBottom(void) { return _clipy2; };
    inline coord_t clipRight(void) { return _clipx2; };
    inline coord_t clipMidHeight(void) { return clipTop() + clipHeight()/2; };
    inline coord_t clipMidWidth(void) { return clipLeft() + clipWidth()/2; };


private:
    static const int BACKLIGHT_PIN = 21;
    static const long BACKLIGHT_FREQUENCY = 50000;

};

extern Screen screen;

#endif
