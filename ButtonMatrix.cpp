#include "ButtonMatrix.h"

void ButtonMatrix::init(coord_t x, coord_t y, coord_t width, coord_t height, int rows, int columns, int maps, ButtonConfig configuration[]) {
  deleteButtons();
  setBounds(x,y,width,height);
  buttonRows = rows;
  buttonColumns = columns;
  buttonMaps = maps;
  buttons = new Button*[totalButtons()];

  coord_t gap = 1;
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
        buttons[i] = new Button(b_x, b_y, b_w, b_h, configuration[i].bgColor, false, configuration[i].labelText, configuration[i].labelFont, configuration[i].labelColor, configuration[i].icon, configuration[i].id);
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

