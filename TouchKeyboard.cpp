#include "TouchKeyboard.h"
#include "Icons.h"
#include "Debug.h"

void TouchKeyboard::init(coord_t x, coord_t y, coord_t w, coord_t h,color_t color, font_t legendFont, color_t legendColor)
{
  setBounds(x,y,w,h);
  colored = color;
  legendF = legendFont;
  legendC = legendColor;
  setKeySet(LOWER_SET);
}

//////////////////////////////////////////////////////
void TouchKeyboard::initButtons() {
  DEBUG_LN("initButtons");
  coord_t buttonDistance = min(width/maxKeysPerRow, height/rows);
  DEBUGF("buttonDistance: %d\n",buttonDistance);
  coord_t radius = (buttonDistance-2)/2;  // leave a tiny space between
  DEBUGF("radius: %d\n",radius);

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
      icon_t icon = nullptr;
      DEBUGF("button init: %d\n", key);
      switch (key) {
        case SHIFTDOWN_KEY:
        case SHIFTUP_KEY:
          icon = shiftIcon;
          break;
        default:
          icon = nullptr;
      }
      buttons[r][c].init(keyX,keyY,radius,colored,false,keyStr,legendF,legendC,icon);
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

uint8_t TouchKeyboard::key(coord_t x, coord_t y) {
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

void TouchKeyboard::draw() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < keysInRow(r); c++) {
      buttons[r][c].draw();
    }
  }
}

uint8_t TouchKeyboard::down() {
  uint8_t keydown = 0;
  if (pad.down(SCREEN_PAD)) {
    keydown = key(pad.x(), pad.y());
    if (keydown) { sound.click(); }
  }
  return keydown;
};

void TouchKeyboard::setKeySet(KeySet newSet) {
  currSet = newSet;
  initButtons();
  draw();
}

uint8_t TouchKeyboard::up() {
  uint8_t keyup = 0;

  if (pad.up(SCREEN_PAD)) {
    keyup = key(pad.x(), pad.y());
    if (keyup) { sound.click(); }
    switch (keyup) {
      case SHIFTUP_KEY:
        setKeySet(UPPER_SET);
        currSet = UPPER_SET;
        initButtons();
        draw();
        break;
      case SHIFTDOWN_KEY:
        currSet = LOWER_SET;
        initButtons();
        draw();
        break;
      default:
        return keyup;
        break;
    }

  }
  return 0;
};



