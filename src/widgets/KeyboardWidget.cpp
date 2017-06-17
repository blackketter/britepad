#include "widgets/KeyboardWidget.h"
#include "widgets/Button.h"

KeyboardWidget::KeyboardWidget(KeyMatrix* keymatrix, coord_t x, coord_t y, coord_t w, coord_t h, color_t defaultColor, color_t defaultLabelColor) {
  _keymatrix = keymatrix;
  setBounds(x,y,w,h);

  keyswitch_t totalKeys = _keymatrix->numKeys();
  coord_t oneKeyWidth = w/_keymatrix->getWidth();
  coord_t oneKeyHeight = h/_keymatrix->getHeight();

  for (keyswitch_t k = 0; k < totalKeys; k++) {
    if (_keymatrix->getKeyLayout(k)) {
      coord_t button_x = x+_keymatrix->getKeyX(k)*oneKeyWidth;
      coord_t button_y = y+_keymatrix->getKeyY(k)*oneKeyHeight;
      coord_t button_w = oneKeyWidth*_keymatrix->getKeyWidth(k);
      coord_t button_h = oneKeyWidth*_keymatrix->getKeyHeight(k);
      Button* b = new Button(button_x+1, button_y+1, button_w-1, button_h-1, defaultColor, false, "", Arial_10_Bold, defaultLabelColor, nullptr, (widgetid_t)k);
      add(b);
    }
  }
}

void KeyboardWidget::draw() {
  Button* button = (Button*)getNext();
  int i = 0;
  while (button) {
    keyswitch_t k = (keyswitch_t)button->getID();

    keycode_t c = _keymatrix->getCode(k);

    const char* title = _keymatrix->getKeyLabel(c);
    const icon_t icon = _keymatrix->getKeyIcon(c);
    bool down = _keymatrix->keyIsDown(c);

    if (icon) {
      title = nullptr;
    }

    bool changed = false;
    if (button->getTitle() != title) changed = true;
    if (button->getIcon().getData() != icon) changed = true;
    if (button->getHighlighted() != down) changed = true;

    if (changed) {
      button->setHighlighted(down);
      button->setTitle(title);
      button->setIcon(icon);
      button->draw();
      if (i == 2) {
        i = 0;
        // idle every 3rd button
        britepad.idle();
      } else {
        i++;
      }
    }
    button = (Button*)(button->getNext());
  }
}