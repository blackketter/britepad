#include <ILI9341_t3.h>

class Screen : public ILI9341_t3 {
  public:

    const int red = ILI9341_RED;
    const int black = ILI9341_BLACK;
    const int green = ILI9341_GREEN;

    Screen(uint8_t _CS, uint8_t _DC, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9341_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO) {};

    void drawText(const char* text, int x = -1, int y = -1);
    void drawTextF(const char* format, ...);
};
