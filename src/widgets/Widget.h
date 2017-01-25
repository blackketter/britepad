#ifndef _Widget_
#define _Widget_

#include "Screen.h"

typedef uint32_t widgetid_t;

class Widget {

  public:
    virtual ~Widget() {};
    virtual void setBounds(coord_t x, coord_t y, coord_t w, coord_t h) { xpos = x; ypos = y; width = w; height = h; }
    virtual bool hit(coord_t x, coord_t y) { return visible && (x >= xpos) && (x <= xpos+width) && (y > ypos) && (y <= ypos + height); }
    virtual void draw() = 0;

    coord_t getWidth() { return width; }
    coord_t getHeight() { return height; }
    coord_t getRight() { return xpos+width; }
    coord_t getBottom() { return ypos+height; }
    coord_t getTop() { return ypos; }
    coord_t getLeft() { return xpos; }
    coord_t getMidWidth() { return xpos+width/2; }
    coord_t getMidHeight() { return ypos+height/2; }
    void getBounds(rect_t& r) { r.x = xpos; r.y = ypos; r.w = width; r.h = height; };

    widgetid_t getID() { return id; }
    void setID(widgetid_t newID) { id = newID; }

    // todo: make setting the visibility cause a redraw or erase
    virtual void setVisible(bool visibility) { visible = visibility; }
    bool getVisible() { return visible; }

  protected:
    widgetid_t id = 0;
    coord_t xpos = 0;
    coord_t ypos = 0;
    coord_t width = 0;
    coord_t height = 0;
    bool visible = true;
};

#endif
