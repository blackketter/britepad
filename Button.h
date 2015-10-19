#ifndef _Button_
#define _Button_

#include "BritepadShared.h"

class Button {
  public:
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nil, font_t f = Arial_9_Bold, color_t titleColor = screen.black);

    virtual void draw(void);

    virtual bool down(void);
    virtual bool up(void);
    virtual bool hold(void);

    virtual void setColor(color_t newColor) {  colored = newColor; draw(); }
    virtual void setTitle(const char* newTitle) { titleStr = newTitle; draw(); }
    // todo: make setting the visibility cause a redraw or erase
    virtual void setVisible(bool visibility) { visible = visibility; }

    virtual bool hit(coord_t x, coord_t y) { return visible && (x >= xpos) && (x <= xpos+width) && (y > ypos) && (y <= ypos + height); }
    virtual void track();

  protected:
    virtual void drawbg();
    virtual void drawTitle();

    virtual color_t fillColor();


    virtual bool getHighlighted(void) { return highlighted; };
    virtual void setHighlighted(bool highlight) { highlighted = highlight; highlightedTime = pad.time(); draw(); };

    const char* titleStr;

    font_t titleFont;
    color_t titleCol;
    coord_t xpos;
    coord_t ypos;
    coord_t width;
    coord_t height;

    color_t colored;
    bool highlighted;
    millis_t highlightedTime;
    bool visible;
    static const millis_t holdTime = 1000;
};

class RoundButton : public Button {
  public:
   virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nil, font_t f = Arial_9_Bold, color_t titleColor = screen.black);

  protected:
    virtual void drawbg();

};
#endif

