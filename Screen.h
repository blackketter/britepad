#ifndef _SCREEN_
#define _SCREEN_

#include <ILI9341_t3.h>

typedef uint16_t color_t;

enum screen_dir {
  UP, RIGHT, DOWN, LEFT
};

class Screen : public ILI9341_t3 {
  public:


    const int red = ILI9341_RED;
    const int black = ILI9341_BLACK;
    const int green = ILI9341_GREEN;
    const int blue = ILI9341_BLUE;
    const int white = ILI9341_WHITE;
    const int yellow = ILI9341_YELLOW;

    const int darkred =   0x8000; // .5, 0, 0
    const int darkgreen = 0x0400; // 0, .5, .0
    const int darkblue =  0x0010; // 0, 0, .5

    const int darkerred =   0x4000; // .25, 0, 0
    const int darkergreen = 0x0200; // 0, .25, .0
    const int darkerblue =  0x0008; // 0, 0, .s5

    const int bluegreen = 0x0410; // 0,  .5, .5

    Screen(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9341_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO) {
    };

    void drawText(const char* text);
    void drawTextF(const char* format, ...);
    int measureTextH(const char* text);
    int measureTextV(const char* text);
    void setJustify(int j) {};

    void pushFill(screen_dir dir, color_t color);
protected:
};

#endif
