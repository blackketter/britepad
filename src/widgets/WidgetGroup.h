#ifndef _WidgetGroup_
#define _WidgetGroup_
#include "Widget.h"

class WidgetGroup : public Widget {
  public:
    ~WidgetGroup();  // this recursively deletes any added widgets

    virtual void draw();
    virtual Widget* getByID(widgetid_t id);
    virtual Widget* pressedWidget();
    virtual Widget* releasedWidget();
    virtual Widget* heldWidget();
    bool pressed() { return pressedWidget() != nullptr; }
    bool released() { return releasedWidget() != nullptr; }
    bool held() { return heldWidget() != nullptr; }
};
#endif // _WidgetGroup_