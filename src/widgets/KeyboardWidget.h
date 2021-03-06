#ifndef _KeyboardWidget_
#define _KeyboardWidget_
#include "widgets/Widget.h"
#include "KeyMatrix.h"

class KeyboardWidget : public Widget {
  public:
    KeyboardWidget(KeyMatrix* keymatrix, coord_t x, coord_t y, coord_t w, coord_t h, color_t defaultColor, color_t defaultLabelColor);
    virtual void draw();
  private:
    KeyMatrix* _keymatrix;
};
#endif