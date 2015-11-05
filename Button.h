#ifndef _Button_
#define _Button_

#include "BritepadShared.h"
#include "Icon.h"

class Button {
  public:
    Button() {};
    Button(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
      init(x,y,w,h,color,highlight,title,f,titleColor,iconData);
    };

    Button(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
     init(x,y,r,color,highlight,title,f,titleColor,iconData);
    };

    // constructor for positioning later
    Button(color_t color, const char* title, font_t f, color_t titleColor, icon_t iconData, uint8_t id) {
      init(0,0,0,0,color,false,title,f,titleColor,iconData);
      setID(id);
    };

    virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr);
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr);

    virtual void draw();

    virtual bool down();
    virtual bool up();
    virtual bool hold();

    virtual void setColor(color_t newColor) {  colored = newColor; }
    virtual void setTitle(const char* newTitle) { titleStr = newTitle; }
    virtual void setTitleFont(font_t newFont) { titleFont = newFont; }
    virtual void setIcon(uint8_t* iconptr) { icon = iconptr; }
    virtual void setBounds(coord_t x, coord_t y, coord_t w, coord_t h) { xpos = x; ypos = y; width = w; height = h; }

    // todo: make setting the visibility cause a redraw or erase
    virtual void setVisible(bool visibility) { visible = visibility; }

    virtual void setID(uint8_t newid) { id = newid; };
    virtual uint8_t getID() { return id; }
    virtual bool hit(coord_t x, coord_t y) { return visible && (x >= xpos) && (x <= xpos+width) && (y > ypos) && (y <= ypos + height); }
    virtual void track();

  protected:
    virtual void drawbg();
    virtual void drawTitle();
    virtual void drawIcon();

    virtual color_t fillColor();


    virtual bool getHighlighted() { return highlighted; };
    virtual void setHighlighted(bool highlight) { highlighted = highlight; highlightedTime = pad.time(); draw(); };

    const char* titleStr;

    font_t titleFont;
    color_t titleCol;
    coord_t xpos;
    coord_t ypos;
    coord_t width;
    coord_t height;
    Icon icon;

    color_t colored;
    bool highlighted;
    millis_t highlightedTime;
    bool visible;
    uint8_t id;
    static const millis_t holdTime = 1000;
};

class RoundButton : public Button {
  public:

  protected:
    virtual void drawbg();

};

class RoundedButton : public Button {
  public:
    void setRadius(coord_t r) { radius = r; }
  protected:
    virtual void drawbg();
    uint8_t radius = 8;
};

#endif

