#ifndef _ButtonMatrix_
#define _ButtonMatrix_
#include "widgets/Button.h"

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
    ButtonMatrix() {};
    ButtonMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[] = nullptr) {
      init(x,y,w,h,rows,columns,maps,configuration);
    };

    virtual ~ButtonMatrix() { deleteButtons(); }

    virtual void init(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[] = nullptr);
    virtual void setBounds(coord_t x, coord_t y, coord_t w, coord_t h);

    void deleteButtons() {
       if (buttons) {
        for (int i = 0; i < totalButtons(); i++) {
          if ( buttons[i]) {
            delete buttons[i];
          }
        };
        delete buttons;
        buttons = nullptr;
      }
    }
    Button* down();
    Button* up();
    Button* hold();
    void draw();

    void setMap(int newMap) { currMap = newMap; }
    int getMap() { return currMap; };

    Button* getButton(widgetid_t id);
    Button* getButton(int i, int map = 0) {  if (buttons) { return buttons[i+map*buttonRows*buttonColumns]; } else { return nullptr; } }

    void setButton(Button* b, int i, int map = 0);

    int rowColToIndex(int r, int c) { return r*buttonColumns+c; }

    virtual void setHighlighted(bool highlight);

  protected:
    int index(int row, int column, int map) { return map*buttonRows*buttonColumns + row*buttonColumns + column; };
    int totalButtons() { return buttonRows*buttonColumns*buttonMaps; }
    void updateButtonBounds(int index);

    Button** buttons = nullptr;
    int buttonRows = 0;
    int buttonColumns = 0;
    int buttonMaps = 0;
    int currMap = 0;
    virtual Button* newButton() { return new Button(); }
    static const coord_t gap = 1;
};

class RoundButtonMatrix : public ButtonMatrix {
  public:
    RoundButtonMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[] = nullptr) {
      init(x, y, w, h, rows, columns, maps, configuration);
    };

  protected:
    Button* newButton() { return new RoundButton(); }
};
#endif
