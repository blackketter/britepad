#ifndef _Button_
#define _Button_

#include "BritepadShared.h"
#include "Widget.h"
#include "widgets/Icon.h"

class Button : public Widget {
  public:
    Button() {};
    Button(coord_t x, coord_t y, coord_t w, coord_t h,color_t color = screen.black, bool highlight = false, const char* title = nullptr, font_t f = &Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
      init(x,y,w,h,color,highlight,title,f,titleColor,iconData);
    };

    Button(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight, const char* title, font_t f, color_t titleColor, icon_t iconData, widgetid_t id) {
      init(x,y,w,h,color,highlight,title,f,titleColor,iconData, id);
    };

    Button(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = &Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr) {
     init(x,y,r,color,highlight,title,f,titleColor,iconData);
    };

    // constructor for positioning later
    Button(color_t color, const char* title, font_t f, color_t titleColor, icon_t iconData, widgetid_t id) {
      init(0,0,0,0,color,false,title,f,titleColor,iconData);
      setID(id);
    };

    virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight = false, const char* title = nullptr, font_t f = &Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr);
    virtual void init(coord_t x, coord_t y, coord_t r,color_t color, bool highlight, color_t iconColor, icon_t iconData) {
      init(x,y,r,color,highlight,(const char*)nullptr,&Arial_9_Bold,iconColor,iconData);
    }
    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nullptr, font_t f = &Arial_9_Bold, color_t titleColor = screen.black, icon_t iconData = nullptr, widgetid_t id = 0, keycode_t key = NO_CODE);

    virtual bool pressed();
    virtual bool released();
    virtual bool held();

    virtual void setColor(color_t newColor) {  colored = newColor; }
    virtual color_t getColor() { return colored; }

    virtual void setTitle(const char* newTitle) { titleStr = newTitle; }
    virtual const char* getTitle() { return titleStr; }
    virtual void setTitleFont(font_t newFont) { titleFont = newFont; }
    virtual void setIcon(Icon iconptr) { icon = iconptr; }
    virtual Icon getIcon() { return icon; }

    virtual font_t getTitleFont() { return titleFont; }
    virtual color_t getTitleColor() { return titleCol; }

    void setFrameInset(coord_t frameInset) { _frameInset = frameInset; }
    void setWrapTitle(bool wrap) { _wrapTitle = wrap; }

    virtual void track();

    void draw();

    virtual bool getHighlighted() { return highlighted; };
    virtual void setHighlighted(bool highlight) { highlighted = highlight; highlightedTime = pad.time(); draw(); };

    bool getSelected() { return selected; };
    void setSelected(bool select) { selected = select;}

  protected:
    virtual void drawbg();
    virtual void drawTitle();
    virtual void drawIcon();

    virtual color_t fillColor();

    static const millis_t holdTime = 1000;


    const char* titleStr;
    font_t titleFont;
    color_t titleCol;
    coord_t _frameInset = 0;
    bool _wrapTitle = false;
    Icon icon;
    color_t colored;
    bool highlighted = false;
    bool selected = false;
    millis_t highlightedTime;
};

class RoundButton : public Button {
  public:

  protected:
    virtual void drawbg();

};

class RoundedButton : public Button {
  public:
    void setRadius(coord_t r) { _radius = r; }
  protected:
    virtual void drawbg();
    coord_t _radius = -1;
};

#endif

