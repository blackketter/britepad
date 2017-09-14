#include "widgets/KeyboardWidget.h"
#include "widgets/Button.h"

KeyboardWidget::KeyboardWidget(KeyMatrix* keymatrix, coord_t x, coord_t y, coord_t w, coord_t h, color_t defaultColor, color_t defaultLabelColor) {
  _keymatrix = keymatrix;
  setBounds(x,y,w,h);

  coord_t oneKeyWidth = w/_keymatrix->getWidth();
  coord_t oneKeyHeight = h/_keymatrix->getHeight();
  console.debugf("w: %d h: %d / matrix w: %d matrix h: %d\n", w, h, _keymatrix->getWidth(), _keymatrix->getHeight());
  if (oneKeyWidth > oneKeyHeight) { oneKeyWidth = oneKeyHeight; }
  if (oneKeyHeight > oneKeyWidth) { oneKeyHeight = oneKeyWidth; }

  int i = 0;
  keyswitch_t k = _keymatrix->getNthKey(i);
  console.debugf("matrix: %d, oneKeyWidth: %d oneKeyHeight: %d\n", _keymatrix, oneKeyWidth, oneKeyHeight);
  console.debugf("first key: %d\n", k);
  while (k != NO_KEY) {
    console.debugf("adding key: %d\n", k);
    if (_keymatrix->getKeyLayout(k)) {
      coord_t button_x = x+_keymatrix->getKeyX(k)*oneKeyWidth;
      coord_t button_y = y+_keymatrix->getKeyY(k)*oneKeyHeight;
      coord_t button_w = oneKeyWidth*_keymatrix->getKeyWidth(k);
      coord_t button_h = oneKeyWidth*_keymatrix->getKeyHeight(k);
      Button* b = new Button(button_x+1, button_y+1, button_w-1, button_h-1, defaultColor, false, "", Arial_10_Bold, defaultLabelColor, nullptr, (widgetid_t)k);
      addWidget(b);
    }
    i++;
    k = _keymatrix->getNthKey(i);
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
    bool down = _keymatrix->switchIsDown(k) || keyEvents.keyIsDown(c);

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
        // drawing may be slow, so idle every 3rd button
        britepad.idle();
      } else {
        i++;
      }
    }
    button = (Button*)(button->getNext());
  }
}