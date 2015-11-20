#ifndef _Button_
#define _Button_

#include "BritepadShared.h"
#include "Widget.h"
#include "Icon.h"

class Button : public Widget {
  public:
    Button() {};
    Button(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
      init(x,y,w,h,color,highlight,title,f,titleColor,iconData);
    };

    Button(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, icon_t iconData, widgetid_t id) {
      init(x,y,w,h,color,highlight,title,f,titleColor,iconData, id);
    };

    Button(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
     init(x,y,r,color,highlight,title,f,titleColor,iconData);
    };

    // constructor for positioning later
    Button(color_t color, const char* title, font_t f, color_t titleColor, icon_t iconData, widgetid_t id) {
      init(0,0,0,0,color,false,title,f,titleColor,iconData);
      setID(id);
    };

    virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr);
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nullptr, font_t f = Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr, widgetid_t id = 0);

    virtual bool down();
    virtual bool up();
    virtual bool hold();

    virtual void setColor(color_t newColor) {  colored = newColor; }
    virtual void setTitle(const char* newTitle) { titleStr = newTitle; }
    virtual void setTitleFont(font_t newFont) { titleFont = newFont; }
    virtual void setIcon(uint8_t* iconptr) { icon = iconptr; }


    virtual uint8_t getID() { return id; }
    virtual void track();

    void draw();

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

    Icon icon;

    color_t colored;
    bool highlighted;
    millis_t highlightedTime;
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
    coord_t radius = 8;
};

#endif

