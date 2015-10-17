#include "TouchKeyboard.h"
#include "Debug.h"

void TouchKeyboard::init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, font_t legendFont, color_t legendColor)
{
  xpos = x;
  ypos = y;
  width = w;
  height = h;
  colored = color;
  legendF = legendFont;
  legendC = legendColor;

  initButtons();
  draw();
}

//////////////////////////////////////////////////////
void TouchKeyboard::initButtons() {
  DEBUG_LN("initButtons");
  coord_t buttonDistance = min(width/maxKeysPerRow, height/rows);
  DEBUG_PARAM_LN("buttonDistance",buttonDistance);
  coord_t radius = (buttonDistance-2)/2;  // leave a tiny space between
  DEBUG_PARAM_LN("radius",radius);

  // initialize all those buttons
  for (int r = 0; r < rows; r++) {
    int rowLen = keysInRow(r);
//    coord_t keyY = ypos + height/rows*r                             + height/rows/2;
      coord_t keyY = ypos + ((long)buttonDistance * 866L * r) / 1000L + height/rows/2;  // .8660 is sqrt(3)/2

    for (int c = 0; c < rowLen; c++) {
      char key = keymap[currSet][r][c];
      char* keyStr = (char*)((uint32_t)key);
      coord_t keyX = xpos + buttonDistance/2 + buttonDistance*c;
      keyX += r % 2 ? buttonDistance/2 : 0;  // every other line is offset by half the distance to nest them together
      DEBUG_PARAM_LN("button init", key);
      buttons[r][c].init(keyX,keyY,radius,colored,false,keyStr,legendF,legendC);
    }
  }
};

int TouchKeyboard::keysInRow(int rowNum) {
  return strlen(keymap[currSet][rowNum]);
}

uint8_t TouchKeyboard::key(int row, int col) {
  const char* rowStr = keymap[currSet][row];

  return rowStr[col];
};

uint8_t TouchKeyboard::hit(coord_t x, coord_t y) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < keysInRow(r); c++) {
      if (buttons[r][c].hit(x,y)) {
        return key(r,c);
      }
    }
  }
  return NO_KEY;
};

void TouchKeyboard::track() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < keysInRow(r); c++) {
      buttons[r][c].track();
    }
  }
};

void TouchKeyboard::draw(void) {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < keysInRow(r); c++) {
      DEBUG_PARAM_LN("button draw", c);
      buttons[r][c].draw();
    }
  }
}

uint8_t TouchKeyboard::down(void) {
  uint8_t keydown = 0;
  if (pad.down(SCREEN_PAD)) {
    keydown = hit(pad.x(), pad.y());
  }
  return keydown;
};

uint8_t TouchKeyboard::up(void) {
  uint8_t keyup = 0;
  if (pad.up(SCREEN_PAD)) {
    keyup = hit(pad.x(), pad.y());
    if (keyup) { sound.click(); }
  }
  return keyup;
};



