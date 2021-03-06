#ifndef _Screen_
#define _Screen_

#if TEENSY == 1
#if defined(LCD_ILI9488)
#include "ILI9488_t3.h"
#elif defined(LCD_ERTFTM0784)
#include "ER-TFT0784_t3.h"
#else
#include "ILI9341_t3.h"
#endif

#else
#include "Adafruit_ILI9341.h"
#endif

#include "Hardware.h"
#include "string.h"
#include "HTMLColor.h"

#define R8(x) ((uint8_t)((((uint16_t)x)&0xf800)>>8))
#define G8(x) ((uint8_t)((((uint16_t)x)&0x07e0)>>3))
#define B8(x) ((uint8_t)((((uint16_t)x)&0x001f)<<3))

#define C32to16(x) (color_t)((((x)&0x00f80000)>>8)+(((x)&0x0000fc00)>>5)+(((x)&0x000000f8)>>3))
#define RGBtoC16(r,g,b) ((color_t)((((r)>>3)<<11)+(((g)>>2)<<5)+((b)>>3)))
#define grey8toC16(x) RGBtoC16((x),(x),(x))

typedef uint16_t color_t;
typedef int16_t coord_t;

typedef struct point_t {
  coord_t x;
  coord_t y;
} point_t;

typedef struct rect_t {
  coord_t x;
  coord_t y;
  coord_t w;
  coord_t h;
} rect_t;


enum direction_t {
  DIRECTION_UP,
  DIRECTION_RIGHT,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_N = DIRECTION_UP,
  DIRECTION_E = DIRECTION_RIGHT,
  DIRECTION_S = DIRECTION_DOWN,
  DIRECTION_W = DIRECTION_LEFT,
  DIRECTION_NE,
  DIRECTION_SE,
  DIRECTION_SW,
  DIRECTION_NW
};

#if TEENSY == 1

//typedef ILI9341_t3_font_t font_t;
typedef const ILI9341_t3_font_t* font_t;

#if defined(LCD_ILI9488)
class Screen : public ILI9488_t3
{
  public:
    Screen(uint8_t _CS = TFT_CS_PIN, uint8_t _DC = TFT_DC_PIN, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9488_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO)

#elif defined(LCD_ERTFTM0784)
class Screen : public ER_TFT0784_t3
{
  public:
     Screen(
        uint8_t _CS = TFT_CS_PIN,
        uint8_t _RESET = TFT_RESET_PIN,
        uint8_t _MOSI = TFT_MOSI_PIN,
        uint8_t _SCLK = TFT_SCLK_PIN,
        uint8_t _MISO = TFT_MISO_PIN,
        uint8_t _2828CS = TFT_2828CS_PIN,
        uint8_t _2828RESET = TFT_2828RESET_PIN,
        uint8_t _2828SDI = TFT_2828SDI_PIN,
        uint8_t _2828SCK = TFT_2828SCK_PIN
      ) :
      ER_TFT0784_t3(_CS, _RESET, _MOSI, _SCLK, _MISO, _2828CS, _2828RESET, _2828SDI, _2828SCK)

#else

class Screen : public ILI9341_t3
{
  public:
    Screen(uint8_t _CS = TFT_CS_PIN, uint8_t _DC = TFT_DC_PIN, uint8_t _RST = 255, uint8_t _MOSI=11, uint8_t _SCLK=13, uint8_t _MISO=12) :
      ILI9341_t3(_CS, _DC, _RST, _MOSI, _SCLK, _MISO)
#endif

#else

//this is for ESP8266 Wemos D1 Mini
typedef GFXfont font_t;

class Screen : public Adafruit_ILI9341
{
  public:
    Screen(uint8_t _CS = TFT_CS_PIN, uint8_t _DC = TFT_DC_PIN, uint8_t _RST = 255) :
      Adafruit_ILI9341(_CS, _DC, _RST)

#endif

    {
      pinMode(BACKLIGHT_PIN, OUTPUT);
      digitalWrite(BACKLIGHT_PIN, LOW);   // set the backlight off
    };
#if defined(LCD_ERTFTM0784)
    static const uint8_t maxbrightness = 255;
#else
    static const uint8_t maxbrightness = 255;
#endif
    static const coord_t offscreen = -100;
    void setBacklight(uint8_t brightness);
    uint8_t getBacklight();

    // inline utility functions that use point_t and rect_t typedefs
    void fillRectx(rect_t& r, color_t c) { fillRect(r.x, r.y, r.w, r.h, c); }
    void drawLinex(point_t& f, point_t& t, color_t c) { drawLine(f.x,f.y,t.x,t.y,c); }

    void drawTextF(const char* format, ...);
    void drawString( String& t) { if (t && t.c_str()) drawText(t.c_str()); }
    void softWrapText(String& out, const char* in);

    void setFont(font_t f) {
#if defined(LCD_ILI9488)
      ILI9488_t3::setFont(*f);
#elif defined(LCD_ERTFTM0784)
      RA8876_t3::setFont(*f);
#else
      Adafruit_ILI9341::setFont(*f);
#endif
    }
    void drawWideLine(coord_t x0, coord_t y0, coord_t x1, coord_t y1, coord_t width, color_t color);

    void pushFill(direction_t dir, color_t color);

    static color_t darken(color_t c);
    static color_t brighten(color_t c, uint8_t offset = 64);
    static color_t mix(color_t c1, color_t c2);
    static color_t add(color_t c1, color_t c2);
    static uint8_t luminance(color_t);
    static void polarToRect(uint16_t theta, coord_t r, coord_t& x, coord_t& y);

    inline coord_t clipTop() { return _clipy1; };
    inline coord_t clipLeft() { return _clipx1; };
    inline coord_t clipHeight() { return _clipy2 - _clipy1; };
    inline coord_t clipWidth() { return _clipx2 - _clipx1; };
    inline coord_t clipBottom() { return _clipy2; };
    inline coord_t clipRight() { return _clipx2; };
    inline coord_t clipMidHeight() { return clipTop() + clipHeight()/2; };
    inline coord_t clipMidWidth() { return clipLeft() + clipWidth()/2; };
    inline void getScreenBounds(rect_t* r) { r->x = 0; r->y = 0; r->h = height(); r->w = width(); }

    // this pushClipRect sets the cliprect to the passed rect _AND_ saves the old clip rect back into it.  call setClipRect(r) again to reset.
    inline void pushClipRect(rect_t* r) { rect_t t; t.x = clipLeft(); t.y = clipTop(); t.h = clipHeight(); t.w = clipWidth(); setClipRect((int16_t)(r->x),(int16_t)(r->y),(int16_t)(r->w),(int16_t)(r->h)); r->y = t.y; r->x = t.x; r->h = t.h; r->w = t.w; };

    inline void getOrigin(point_t* o) { o->x = _originx; o->y = _originy; }
    inline void setOrigin(point_t* o) { _originx = o->x; _originy = o->y; }

    inline void pushOrigin(point_t* o) { point_t t; getOrigin(&t); setOrigin(o); o->x = t.x; o->y = t.y; };

//    virtual void drawPixel(point_t& p, color_t color) { drawPixel((int16_t)p.x,(int16_t)p.y,(uint16_t)color); };

// Color definitions
    static const color_t black = 0x0000;
    static const color_t white = 0xFFFF;

    static const color_t red = 0xF800;
    static const color_t green = 0x07E0;
    static const color_t blue = 0x001F;
    static const color_t yellow = 0xFFE0;
    static const color_t orange = 0xFD20;

    static const color_t cyan = 0x07ff; // 0, 1, 1
    static const color_t bluegreen = 0x0410; // 0,  .5, .5

    static const color_t grey = 0x4208; // .25, .25, .25
    static const color_t lightgrey = 0x8410;
    static const color_t lightergrey = grey+lightgrey;

    static const color_t darkred =   0x8000; // .5, 0, 0
    static const color_t darkgreen = 0x0400; // 0, .5, .0
    static const color_t darkblue =  0x0010; // 0, 0, .5
    static const color_t darkyellow =  0x8400; // .5, .5., 0

    static const color_t darkerred =   0x4000; // .25, 0, 0
    static const color_t darkergreen = 0x0200; // 0, .25, .0
    static const color_t darkerblue =  0x0008; // 0, 0, .25
    static const color_t darkergrey = 0x2104; // 0.125, .125, .125
    static const color_t darkeryellow = 0x2100; // 0.125, .125, 0

    static const color_t grey10 = grey8toC16((255L*10)/100);
    static const color_t grey20 = grey8toC16((255L*20)/100);
    static const color_t grey30 = grey8toC16((255L*30)/100);
    static const color_t grey40 = grey8toC16((255L*40)/100);
    static const color_t grey50 = grey8toC16((255L*50)/100);
    static const color_t grey60 = grey8toC16((255L*60)/100);
    static const color_t grey70 = grey8toC16((255L*70)/100);
    static const color_t grey80 = grey8toC16((255L*80)/100);
    static const color_t grey90 = grey8toC16((255L*90)/100);

    static const color_t midgreen = darkgreen + darkergreen;
    static const color_t AliceBlue = C32to16(HTMLColor::AliceBlue);
    static const color_t AntiqueWhite = C32to16(HTMLColor::AntiqueWhite);
    static const color_t Aqua = C32to16(HTMLColor::Aqua);
    static const color_t Aquamarine = C32to16(HTMLColor::Aquamarine);
    static const color_t Azure = C32to16(HTMLColor::Azure);
    static const color_t Beige = C32to16(HTMLColor::Beige);
    static const color_t Bisque = C32to16(HTMLColor::Bisque);
    static const color_t Black = C32to16(HTMLColor::Black);
    static const color_t BlanchedAlmond = C32to16(HTMLColor::BlanchedAlmond);
    static const color_t Blue = C32to16(HTMLColor::Blue);
    static const color_t BlueViolet = C32to16(HTMLColor::BlueViolet);
    static const color_t Brown = C32to16(HTMLColor::Brown);
    static const color_t BurlyWood = C32to16(HTMLColor::BurlyWood);
    static const color_t CadetBlue = C32to16(HTMLColor::CadetBlue);
    static const color_t Chartreuse = C32to16(HTMLColor::Chartreuse);
    static const color_t Chocolate = C32to16(HTMLColor::Chocolate);
    static const color_t Coral = C32to16(HTMLColor::Coral);
    static const color_t CornflowerBlue = C32to16(HTMLColor::CornflowerBlue);
    static const color_t Cornsilk = C32to16(HTMLColor::Cornsilk);
    static const color_t Crimson = C32to16(HTMLColor::Crimson);
    static const color_t Cyan = C32to16(HTMLColor::Cyan);
    static const color_t DarkBlue = C32to16(HTMLColor::DarkBlue);
    static const color_t DarkCyan = C32to16(HTMLColor::DarkCyan);
    static const color_t DarkGoldenRod = C32to16(HTMLColor::DarkGoldenRod);
    static const color_t DarkGray = C32to16(HTMLColor::DarkGray);
    static const color_t DarkGreen = C32to16(HTMLColor::DarkGreen);
    static const color_t DarkKhaki = C32to16(HTMLColor::DarkKhaki);
    static const color_t DarkMagenta = C32to16(HTMLColor::DarkMagenta);
    static const color_t DarkOliveGreen = C32to16(HTMLColor::DarkOliveGreen);
    static const color_t DarkOrange = C32to16(HTMLColor::DarkOrange);
    static const color_t DarkOrchid = C32to16(HTMLColor::DarkOrchid);
    static const color_t DarkRed = C32to16(HTMLColor::DarkRed);
    static const color_t DarkSalmon = C32to16(HTMLColor::DarkSalmon);
    static const color_t DarkSeaGreen = C32to16(HTMLColor::DarkSeaGreen);
    static const color_t DarkSlateBlue = C32to16(HTMLColor::DarkSlateBlue);
    static const color_t DarkSlateGray = C32to16(HTMLColor::DarkSlateGray);
    static const color_t DarkTurquoise = C32to16(HTMLColor::DarkTurquoise);
    static const color_t DarkViolet = C32to16(HTMLColor::DarkViolet);
    static const color_t DeepPink = C32to16(HTMLColor::DeepPink);
    static const color_t DeepSkyBlue = C32to16(HTMLColor::DeepSkyBlue);
    static const color_t DimGray = C32to16(HTMLColor::DimGray);
    static const color_t DodgerBlue = C32to16(HTMLColor::DodgerBlue);
    static const color_t FireBrick = C32to16(HTMLColor::FireBrick);
    static const color_t FloralWhite = C32to16(HTMLColor::FloralWhite);
    static const color_t ForestGreen = C32to16(HTMLColor::ForestGreen);
    static const color_t Fuchsia = C32to16(HTMLColor::Fuchsia);
    static const color_t Gainsboro = C32to16(HTMLColor::Gainsboro);
    static const color_t GhostWhite = C32to16(HTMLColor::GhostWhite);
    static const color_t Gold = C32to16(HTMLColor::Gold);
    static const color_t GoldenRod = C32to16(HTMLColor::GoldenRod);
    static const color_t Gray = C32to16(HTMLColor::Gray);
    static const color_t Green = C32to16(HTMLColor::Green);
    static const color_t GreenYellow = C32to16(HTMLColor::GreenYellow);
    static const color_t HoneyDew = C32to16(HTMLColor::HoneyDew);
    static const color_t HotPink = C32to16(HTMLColor::HotPink);
    static const color_t IndianRed = C32to16(HTMLColor::IndianRed);
    static const color_t Indigo = C32to16(HTMLColor::Indigo);
    static const color_t Ivory = C32to16(HTMLColor::Ivory);
    static const color_t Khaki = C32to16(HTMLColor::Khaki);
    static const color_t Lavender = C32to16(HTMLColor::Lavender);
    static const color_t LavenderBlush = C32to16(HTMLColor::LavenderBlush);
    static const color_t LawnGreen = C32to16(HTMLColor::LawnGreen);
    static const color_t LemonChiffon = C32to16(HTMLColor::LemonChiffon);
    static const color_t LightBlue = C32to16(HTMLColor::LightBlue);
    static const color_t LightCoral = C32to16(HTMLColor::LightCoral);
    static const color_t LightCyan = C32to16(HTMLColor::LightCyan);
    static const color_t LightGoldenRodYellow = C32to16(HTMLColor::LightGoldenRodYellow);
    static const color_t LightGray = C32to16(HTMLColor::LightGray);
    static const color_t LightGreen = C32to16(HTMLColor::LightGreen);
    static const color_t LightPink = C32to16(HTMLColor::LightPink);
    static const color_t LightSalmon = C32to16(HTMLColor::LightSalmon);
    static const color_t LightSeaGreen = C32to16(HTMLColor::LightSeaGreen);
    static const color_t LightSkyBlue = C32to16(HTMLColor::LightSkyBlue);
    static const color_t LightSlateGray = C32to16(HTMLColor::LightSlateGray);
    static const color_t LightSteelBlue = C32to16(HTMLColor::LightSteelBlue);
    static const color_t LightYellow = C32to16(HTMLColor::LightYellow);
    static const color_t Lime = C32to16(HTMLColor::Lime);
    static const color_t LimeGreen = C32to16(HTMLColor::LimeGreen);
    static const color_t Linen = C32to16(HTMLColor::Linen);
    static const color_t Magenta = C32to16(HTMLColor::Magenta);
    static const color_t Maroon = C32to16(HTMLColor::Maroon);
    static const color_t MediumAquaMarine = C32to16(HTMLColor::MediumAquaMarine);
    static const color_t MediumBlue = C32to16(HTMLColor::MediumBlue);
    static const color_t MediumOrchid = C32to16(HTMLColor::MediumOrchid);
    static const color_t MediumPurple = C32to16(HTMLColor::MediumPurple);
    static const color_t MediumSeaGreen = C32to16(HTMLColor::MediumSeaGreen);
    static const color_t MediumSlateBlue = C32to16(HTMLColor::MediumSlateBlue);
    static const color_t MediumSpringGreen = C32to16(HTMLColor::MediumSpringGreen);
    static const color_t MediumTurquoise = C32to16(HTMLColor::MediumTurquoise);
    static const color_t MediumVioletRed = C32to16(HTMLColor::MediumVioletRed);
    static const color_t MidnightBlue = C32to16(HTMLColor::MidnightBlue);
    static const color_t MintCream = C32to16(HTMLColor::MintCream);
    static const color_t MistyRose = C32to16(HTMLColor::MistyRose);
    static const color_t Moccasin = C32to16(HTMLColor::Moccasin);
    static const color_t NavajoWhite = C32to16(HTMLColor::NavajoWhite);
    static const color_t Navy = C32to16(HTMLColor::Navy);
    static const color_t OldLace = C32to16(HTMLColor::OldLace);
    static const color_t Olive = C32to16(HTMLColor::Olive);
    static const color_t OliveDrab = C32to16(HTMLColor::OliveDrab);
    static const color_t Orange = C32to16(HTMLColor::Orange);
    static const color_t OrangeRed = C32to16(HTMLColor::OrangeRed);
    static const color_t Orchid = C32to16(HTMLColor::Orchid);
    static const color_t PaleGoldenRod = C32to16(HTMLColor::PaleGoldenRod);
    static const color_t PaleGreen = C32to16(HTMLColor::PaleGreen);
    static const color_t PaleTurquoise = C32to16(HTMLColor::PaleTurquoise);
    static const color_t PaleVioletRed = C32to16(HTMLColor::PaleVioletRed);
    static const color_t PapayaWhip = C32to16(HTMLColor::PapayaWhip);
    static const color_t PeachPuff = C32to16(HTMLColor::PeachPuff);
    static const color_t Peru = C32to16(HTMLColor::Peru);
    static const color_t Pink = C32to16(HTMLColor::Pink);
    static const color_t Plum = C32to16(HTMLColor::Plum);
    static const color_t PowderBlue = C32to16(HTMLColor::PowderBlue);
    static const color_t Purple = C32to16(HTMLColor::Purple);
    static const color_t RebeccaPurple = C32to16(HTMLColor::RebeccaPurple);
    static const color_t Red = C32to16(HTMLColor::Red);
    static const color_t RosyBrown = C32to16(HTMLColor::RosyBrown);
    static const color_t RoyalBlue = C32to16(HTMLColor::RoyalBlue);
    static const color_t SaddleBrown = C32to16(HTMLColor::SaddleBrown);
    static const color_t Salmon = C32to16(HTMLColor::Salmon);
    static const color_t SandyBrown = C32to16(HTMLColor::SandyBrown);
    static const color_t SeaGreen = C32to16(HTMLColor::SeaGreen);
    static const color_t SeaShell = C32to16(HTMLColor::SeaShell);
    static const color_t Sienna = C32to16(HTMLColor::Sienna);
    static const color_t Silver = C32to16(HTMLColor::Silver);
    static const color_t SkyBlue = C32to16(HTMLColor::SkyBlue);
    static const color_t SlateBlue = C32to16(HTMLColor::SlateBlue);
    static const color_t SlateGray = C32to16(HTMLColor::SlateGray);
    static const color_t Snow = C32to16(HTMLColor::Snow);
    static const color_t SpringGreen = C32to16(HTMLColor::SpringGreen);
    static const color_t SteelBlue = C32to16(HTMLColor::SteelBlue);
    static const color_t Tan = C32to16(HTMLColor::Tan);
    static const color_t Teal = C32to16(HTMLColor::Teal);
    static const color_t Thistle = C32to16(HTMLColor::Thistle);
    static const color_t Tomato = C32to16(HTMLColor::Tomato);
    static const color_t Turquoise = C32to16(HTMLColor::Turquoise);
    static const color_t Violet = C32to16(HTMLColor::Violet);
    static const color_t Wheat = C32to16(HTMLColor::Wheat);
    static const color_t White = C32to16(HTMLColor::White);
    static const color_t WhiteSmoke = C32to16(HTMLColor::WhiteSmoke);
    static const color_t Yellow = C32to16(HTMLColor::Yellow);
    static const color_t YellowGreen = C32to16(HTMLColor::YellowGreen);

private:
    static const uint32_t BACKLIGHT_FREQUENCY = 50000;
    uint8_t backlightBrightness = maxbrightness;

};

extern Screen screen;

#endif

