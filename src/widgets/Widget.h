#ifndef _Widget_
#define _Widget_

#include "Screen.h"
#include "KeyMatrix.h"

typedef uint32_t widgetid_t;

class Widget {

  public:
    virtual ~Widget();
    virtual void setBounds(coord_t x, coord_t y, coord_t w, coord_t h) { _xpos = x; _ypos = y; _width = w; _height = h; }
    virtual bool hit(coord_t x, coord_t y) { return _visible && (x >= _xpos) && (x <= _xpos+_width) && (y > _ypos) && (y <= _ypos + _height); }
    virtual void draw();
    virtual bool key(Event* k) { return false; };

    coord_t getWidth() { return _width; }
    coord_t getHeight() { return _height; }
    coord_t getRight() { return _xpos+_width; }
    coord_t getBottom() { return _ypos+_height; }
    coord_t getTop() { return _ypos; }
    coord_t getLeft() { return _xpos; }
    coord_t getMidWidth() { return _xpos+_width/2; }
    coord_t getMidHeight() { return _ypos+_height/2; }
    void getBounds(rect_t& r) { r.x = _xpos; r.y = _ypos; r.w = _width; r.h = _height; };

    widgetid_t getID() { return _id; }
    void setID(widgetid_t newID) { _id = newID; }

    keycode_t getKey() { return _key; }
    void setKey(keycode_t key) { _key = key; }

    // todo: make setting the visibility cause a redraw or erase
    virtual void setVisible(bool visibility) { _visible = visibility; }
    virtual bool getVisible() { return _visible; }
    virtual void setEnabled(bool enable) { _enabled = enable; }
    virtual bool getEnabled() { return _enabled; }

    // manage linked list of widgets
    Widget* getNext() { return _next; }
    void setNext(Widget* n) { _next = n; }
    void addWidget(Widget* w);
    virtual Widget* getByID(widgetid_t id);
    virtual Widget* pressedWidget();
    virtual Widget* releasedWidget();
    virtual Widget* heldWidget();

    virtual bool pressed() { return pressedWidget() != nullptr; }
    virtual bool released() { return releasedWidget() != nullptr; }
    virtual bool held() { return heldWidget() != nullptr; }

  protected:
    widgetid_t _id = 0;
    coord_t _xpos = 0;
    coord_t _ypos = 0;
    coord_t _width = 0;
    coord_t _height = 0;
    bool _visible = true;
    bool _enabled = true;
    keycode_t _key = 0;
    Widget* _next = nullptr;
};

#endif
