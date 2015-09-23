#ifndef _Button_
#define _Button_

#include "BritepadShared.h"

class Button {
  public:
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nil, uint8_t* icon = nil);

    virtual void draw();
    virtual bool hit(coord_t x, coord_t y) { return (x >= xpos) && (x <= xpos+width) && (y > ypos) && (y <= ypos + height); }
    virtual bool down(void);

    virtual bool getHighlighted(void) { return highlighted; };
    virtual void setHighlighted(bool highlight) { highlighted = highlight; draw(); };

    virtual void setColor(color_t newColor) {  colored = newColor; draw(); }
    virtual void setTitle(const char* newTitle) { titleStr = newTitle; draw(); }

  protected:
    virtual void drawbg();
    virtual void drawTitle();
    virtual color_t fillColor();

    const char* titleStr;
    uint8_t* iconData;

    coord_t xpos;
    coord_t ypos;
    coord_t width;
    coord_t height;

    color_t colored;
    bool highlighted;
};

class RoundButton : public Button {
  public:
   virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nil, uint8_t* icon = nil);

  protected:
    virtual void drawbg();

};
#endif

