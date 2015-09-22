#ifndef _Button_
#define _Button_

#include "BritepadShared.h"

class Button {
  public:
    void init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, bool highlight = false, const char* title = nil, uint8_t* icon = nil);

    void draw();
    bool hit(coord_t x, coord_t y) { return (x >= xpos) && (x <= xpos+width) && (y > ypos) && (y <= ypos + height); }
    bool down(void);

    bool getHighlighted(void) { return highlighted; };
    void setHighlighted(bool highlight) { highlighted = highlight; draw(); };

    void setColor(color_t newColor) {  colored = newColor; draw(); }
    void setTitle(const char* newTitle) { titleStr = newTitle; draw(); }

  private:

    const char* titleStr;
    uint8_t* iconData;

    coord_t xpos;
    coord_t ypos;
    coord_t width;
    coord_t height;

    color_t colored;
    bool highlighted;
};

#endif

