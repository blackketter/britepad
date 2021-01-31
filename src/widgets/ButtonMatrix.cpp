#include "widgets/ButtonMatrix.h"

void ButtonMatrix::init(coord_t x, coord_t y, coord_t width, coord_t height, int rows, int columns, int maps, ButtonConfig configuration[]) {
  deleteButtons();
  setBounds(x,y,width,height);
  buttonRows = rows;
  buttonColumns = columns;
  buttonMaps = maps;
  buttons = new Button*[totalButtons()];
  for (buttonindex_t i = 0; i < totalButtons(); i++) {
    buttons[i] = nullptr;
  }
  coord_t xorig = x + gap;
  coord_t yorig = y + gap;
  coord_t xspacing = (width-gap)/columns;
  coord_t yspacing = (height-gap)/rows;
  coord_t b_w = xspacing - gap;
  coord_t b_h = yspacing - gap;
  if (configuration) {
    // lay out button according to how much space we have on the screen
    for (int m = 0; m<buttonMaps;m++) {
      coord_t b_y = yorig;
      for (int r = 0; r<buttonRows;r++) {
        coord_t b_x = xorig;
        for (int c = 0; c<buttonColumns;c++) {
          buttonindex_t i = index(r,c,m);
          buttons[i] = newButton();
          if (configuration) {
            buttons[i]->init(b_x, b_y, b_w, b_h, configuration[i].bgColor, false, configuration[i].labelText, configuration[i].labelFont, configuration[i].labelColor, configuration[i].icon, configuration[i].id, configuration[i].key);
          } else {
            buttons[i]->init(b_x, b_y, b_w, b_h);
          }
          b_x += xspacing;
        }
      b_y += yspacing;
      }
    }
  }
}

void ButtonMatrix::setBounds(coord_t x, coord_t y, coord_t w, coord_t h) {
  Widget::setBounds(x,y,w,h);
  if (buttons) {
    for (buttonindex_t i = 0; i < totalButtons(); i++) {
      updateButtonBounds(i);
    }
  }
}

void ButtonMatrix::draw() {
  for (int m = 0; m < buttonMaps; m++) {
    for (int r = 0; r < buttonRows; r++) {
      for (int c = 0; c < buttonColumns; c++) {
        buttonindex_t i = index(r,c,m);
        if (buttons[i]) {
          bool visible = (currMap == m);
          buttons[i]->setVisible(visible);
          if (visible) {
            buttons[i]->draw();
            launcher.idle();
          }
        }
      }
    }
  }
}

Button* ButtonMatrix::pressedButton() {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      int i = index(r,c,currMap);
      if (buttons[i] && buttons[i]->pressed()) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}


Button* ButtonMatrix::releasedButton() {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      buttonindex_t i = index(r,c,currMap);
      if (buttons[i] && buttons[i]->released()) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}
buttonindex_t ButtonMatrix::buttonCount() {
  buttonindex_t count = 0;
  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      buttonindex_t i = index(r,c,currMap);
      if (buttons[i]) {
        count++;
      }
    }
  }
  return count;
}

Button* ButtonMatrix::heldButton() {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      buttonindex_t i = index(r,c,currMap);
      if (buttons[i] && buttons[i]->held()) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}

Button* ButtonMatrix::getButton(buttonindex_t i, int map) {
  if (buttons && (i >= 0) && (i < totalButtons())) {
    return buttons[i+map*buttonRows*buttonColumns];
  } else {
    return nullptr;
  }
}

Button* ButtonMatrix::getButton(widgetid_t id) {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      buttonindex_t i = index(r,c,currMap);
      if (buttons[i]) {
        console.debugf("Button %d, %d id: %d\n", r,c,buttons[i]->getID());
        if (buttons[i]->getID() == id) {
          return buttons[i];
        }
      }
    }
  }
  return nullptr;
}

Button* ButtonMatrix::keyButton(KeyEvent* event) {
  if (event == nullptr) return nullptr;

  keycode_t code = event->code();
  char character = event->character();

  Button* b = nullptr;

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      int i = index(r,c,currMap);
      if (buttons[i]) {
        keycode_t k = buttons[i]->getKey();
        if (code == k || character == k) {
          b = buttons[i];
          break;
        }
      }
    }
    if (b) break;
  }
  if (b) {
    b->setHighlighted(event->pressed());
  }
  return b;
}

void ButtonMatrix::setHighlighted(bool highlight) {
  for (int m = 0; m < buttonMaps; m++) {
    for (int r = 0; r < buttonRows; r++) {
      for (int c = 0; c < buttonColumns; c++) {
        buttonindex_t i = index(r,c,m);
        if (buttons[i] && buttons[i]->getHighlighted() != highlight) {
          buttons[i]->setHighlighted(highlight);
        }
      }
    }
  }
}

void ButtonMatrix::setButton(Button* b, buttonindex_t i, int map) {
  buttonindex_t p = i+map*buttonRows*buttonColumns;
  if (p >= buttonMaps*buttonColumns*buttonRows) {
    console.debugln("BUTTON OUT OF RANGE, NOT ADDING TO MATRIX");
    return;
  }
  if (buttons[p]) {
    delete(buttons[p]);
  }
  buttons[p]=b;
  updateButtonBounds(p);
}

void ButtonMatrix::updateButtonBounds(buttonindex_t index) {
  if (buttons[index]) {
    buttonindex_t i = index%(buttonRows*buttonColumns);

    coord_t xspacing = (getWidth()-gap)/buttonColumns;
    coord_t yspacing = (getHeight()-gap)/buttonRows;

    coord_t w = xspacing - gap;
    coord_t h = yspacing - gap;

    coord_t x = getLeft() + gap + xspacing*(i%buttonColumns);
    coord_t y = getTop() + gap + yspacing*(i/buttonColumns);

    buttons[index]->setBounds(x, y, w, h);
  }
}

buttonindex_t ButtonMatrix::getSelected() {
  buttonindex_t selected = NO_BUTTON;
  for (buttonindex_t i = 0; i < totalButtons(); i++) {
    if (buttons[i] && buttons[i]->getSelected()) {
      selected = i;
      break;
    }
  }
  return selected;
}

void ButtonMatrix::setSelected(buttonindex_t button) {
  buttonindex_t old = getSelected();
  if (old == button) { return; }
  for (buttonindex_t i = 0; i < totalButtons(); i++) {
    if (buttons[i]) {
      buttons[i]->setSelected(button == i);
    }
  }
}
