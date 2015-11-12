#ifndef _ButtonMatrix_
#define _ButtonMatrix_
#include "Button.h"

struct ButtonConfig {
  color_t bgColor;
  const char* labelText;
  font_t labelFont;
  color_t labelColor;
  icon_t icon;
  widgetid_t id;
};

class ButtonMatrix : public Widget {
  public:
    ButtonMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[]) {
      init(x,y,w,h,rows,columns,maps,configuration);
    };

    virtual ~ButtonMatrix() { deleteButtons(); }

    void init(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[]);
    void deleteButtons() { if (buttons) { for (int i = 0; i < totalButtons(); i++) { delete buttons[i]; }; delete buttons; buttons = nullptr; } }

    Button* down();
    void draw();

    void setMap(int newMap) { if (currMap != newMap) { currMap = newMap; draw();} }
    int getMap() { return currMap; };



  private:
    int index(int row, int column, int map) { return map*buttonRows*buttonColumns + row*buttonColumns + column; };
    int totalButtons() { return buttonRows*buttonColumns*buttonMaps; }

    Button** buttons = nullptr;
    int buttonRows = 0;
    int buttonColumns = 0;
    int buttonMaps = 0;
    int currMap = 0;
};

#endif
