#ifndef _WidgetGroup_
#define _WidgetGroup_
#include "Widget.h"

class WidgetGroup : public Widget {
  public:
    ~WidgetGroup();  // this recursively deletes any added widgets
    virtual void draw();
    Widget* getByID(widgetid_t id);
};
#endif // _WidgetGroup_