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
  keycode_t key;
};

typedef int buttonindex_t;
const buttonindex_t NO_BUTTON = -1;

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
    Button* pressedButton();
    Button* releasedButton();
    Button* heldButton();

    Button* keyButton(KeyEvent* event);

    void draw();
    void draw(buttonindex_t i, int map = 0) { Button* b = getButton(i,map); if (b) { b->draw(); }};

    void setMap(int newMap) { currMap = newMap; }
    int getMap() { return currMap; };

    Button* getButton(widgetid_t id);
    Button* getButton(buttonindex_t i, int map = 0);

    void setButton(Button* b, buttonindex_t i, int map = 0);

    buttonindex_t rowColToIndex(int r, int c) { return r*buttonColumns+c; }

    virtual void setHighlighted(bool highlight);
    buttonindex_t totalButtons() { return buttonRows*buttonColumns*buttonMaps; }
    buttonindex_t buttonCount();
    buttonindex_t getSelected();
    void setSelected(buttonindex_t button);
    void setGap(coord_t g) { gap = g; }

  protected:
    buttonindex_t index(int row, int column, int map) { return map*buttonRows*buttonColumns + row*buttonColumns + column; };
    void updateButtonBounds(buttonindex_t index);

    Button** buttons = nullptr;
    int buttonRows = 0;
    int buttonColumns = 0;
    int buttonMaps = 0;
    int currMap = 0;
    virtual Button* newButton() { return new Button(); }
    coord_t gap = 1;
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
