#ifndef _TextField_
#define _TextField_

#include "BritepadShared.h"
#include "Widget.h"
#include "widgets/Icon.h"

class TextField : public Widget
//, public Print
{
  public:
    TextField() {};
    TextField(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, alignment_t a = ALIGN_DEFAULT) {
      init(x,y,w,h,f,fgColor, bgColor,a);
    };


    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h, font_t f, color_t fgColor, color_t bgColor, alignment_t a = ALIGN_DEFAULT);

    void draw() { if (_text.length()) { draw(_text); } else { draw(nullptr); } };
    void draw(const char* t);
    void draw(String& s) { draw(s.c_str());}

    virtual bool key(KeyEvent* k);

    void setText(String& s) { _text = s; }
    void getText(String& s) { s = _text; };
    void setMaxTextLength(uint16_t len) { _maxLength = len; };

    void setColor(color_t fgColor, color_t bgColor) { _fColor = fgColor; _bColor = bgColor; };
    void setColor(color_t c) { _fColor = _bColor = c; };
    void setFColor(color_t c) { _fColor = c; }

    void setFont(font_t f) { _font = f; }
    void setAlign(alignment_t a) { _align = a; }

//    virtual void write(uint8_t);
//    virtual void write(const char *str);
//    virtual void write(const uint8_t *buffer, size_t size);
  protected:
    font_t _font;
    color_t _fColor;
    color_t _bColor;
    alignment_t _align;
    String _text;
    uint16_t _maxLength = UINT16_MAX;
};
#endif


