#ifndef _TextField_
#define _TextField_

#include "BritepadShared.h"
#include "Widget.h"
#include "widgets/Icon.h"

class TextField : public Widget {
  public:
    TextField() {};
    TextField(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, int8_t hAlign = ALIGN_LEFT, int8_t vAlign = ALIGN_TOP, const char* t = nullptr) {
      init(x,y,w,h,f,fgColor, bgColor,hAlign,vAlign,t);
    };


    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, int8_t hAlign = ALIGN_LEFT, int8_t vAlign = ALIGN_TOP, const char* t = nullptr);

    void draw();
    void draw(const char* t);
    void drawf(const char* format, ...);
    void setColor(color_t fgColor, color_t bgColor) { fColor = fgColor; bColor = bgColor; };
    void setColor(color_t c) { fColor = bColor = c; };
    void setFColor(color_t c) { fColor = c; }

  protected:
    font_t font;
    color_t fColor;
    color_t bColor;
    const char* text;
    int8_t alignH;
    int8_t alignV;
};
#endif


