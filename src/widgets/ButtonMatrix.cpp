#include "widgets/ButtonMatrix.h"

void ButtonMatrix::init(coord_t x, coord_t y, coord_t width, coord_t height, int rows, int columns, int maps, ButtonConfig configuration[]) {
  deleteButtons();
  setBounds(x,y,width,height);
  buttonRows = rows;
  buttonColumns = columns;
  buttonMaps = maps;
  buttons = new Button*[totalButtons()];

  coord_t xorig = x + gap;
  coord_t yorig = y + gap;
  coord_t xspacing = (width-gap)/columns;
  coord_t yspacing = (height-gap)/rows;
  coord_t b_w = xspacing - gap;
  coord_t b_h = yspacing - gap;

  // lay out button according to how much space we have on the screen
  for (int m = 0; m<buttonMaps;m++) {
    coord_t b_y = yorig;
    for (int r = 0; r<buttonRows;r++) {
      coord_t b_x = xorig;
      for (int c = 0; c<buttonColumns;c++) {
        int i = index(r,c,m);
        buttons[i] = newButton();
        if (configuration) {
          buttons[i]->init(b_x, b_y, b_w, b_h, configuration[i].bgColor, false, configuration[i].labelText, configuration[i].labelFont, configuration[i].labelColor, configuration[i].icon, configuration[i].id);
        } else {
          buttons[i]->init(b_x, b_y, b_w, b_h);
        }
        b_x += xspacing;
      }
    b_y += yspacing;
    }
  }
}

void ButtonMatrix::draw() {
  for (int m = 0; m < buttonMaps; m++) {
    for (int r = 0; r < buttonRows; r++) {
      for (int c = 0; c < buttonColumns; c++) {
        int i = index(r,c,m);
        buttons[i]->setVisible(currMap == m);
        buttons[i]->draw();
      }
    }
  }
}

Button* ButtonMatrix::down() {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      int i = index(r,c,currMap);
      if (buttons[i]->down()) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}


Button* ButtonMatrix::up() {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      int i = index(r,c,currMap);
      if (buttons[i]->up()) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}

Button* ButtonMatrix::getButton(widgetid_t id) {

  for (int r = 0; r < buttonRows; r++) {
    for (int c = 0; c < buttonColumns; c++) {
      int i = index(r,c,currMap);
      DEBUGF("Button %d, %d id: %d\n", r,c,buttons[i]->getID());
      if (buttons[i]->getID() == id) {
        return buttons[i];
      }
    }
  }
  return nullptr;
}

void ButtonMatrix::setHighlighted(bool highlight) {
  for (int m = 0; m < buttonMaps; m++) {
    for (int r = 0; r < buttonRows; r++) {
      for (int c = 0; c < buttonColumns; c++) {
        int i = index(r,c,m);
        if (buttons[i]->getHighlighted() != highlight) {
          buttons[i]->setHighlighted(highlight);
        }
      }
    }
  }
}
