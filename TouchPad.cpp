#include "Arduino.h"

#include <Adafruit_FT6206.h>

#include "TouchPad.h"

#define DEBUG_ON 1
#include "Debug.h"

#define B_TOUCH_PIN A2
#define T_TOUCH_PIN A8
#define L_TOUCH_PIN A1
#define R_TOUCH_PIN A3

#define L_HIGH_THRESHOLD 950
#define L_LOW_THRESHOLD 850

#define R_HIGH_THRESHOLD 1250
#define R_LOW_THRESHOLD 1150

#define B_HIGH_THRESHOLD 1100
#define B_LOW_THRESHOLD 1000

#define T_HIGH_THRESHOLD 1150
#define T_LOW_THRESHOLD 1050

Adafruit_FT6206 ctp = Adafruit_FT6206();


void copyTPState( TPState* dest, TPState* src ) {
  memcpy(dest,src,sizeof(TPState));
}

TouchPad::TouchPad(int w, int h) {

  height = h;
  width = w;

}

void TouchPad::begin() {
  if (! ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    DEBUG_LN("Couldn't start FT6206 touchscreen controller");
    while (1);
  }
  DEBUG_LN("Started TouchPad");
}

void TouchPad::update() {

  // save the last state for future reference
  copyTPState(&last, &curr);

  curr.time = millis();
  curr.touched[SCREEN_PAD] = ctp.touched();

  // Retrieve a point
  TS_Point p = ctp.getPoint();

  curr.x = p.y;
  curr.y = p.x;

  // bogus coordinates at 0,0
  if (curr.x == 0 && curr.y == 0 && curr.touched[SCREEN_PAD]) {
    DEBUG_LN("bogus 0,0 coordinates");
    curr.touched[SCREEN_PAD] = 0;
  }

  // flip it around to match the screen.
  curr.y = map(curr.y, 0, width, width, 0);

  // if we're not touching, use the last touch coordinates
  if (!curr.touched[SCREEN_PAD]) {
    curr.x = last.x;
    curr.y = last.y;
  }

  // read the 4 touch panels
  int t = touchRead(T_TOUCH_PIN);
  int b = touchRead(B_TOUCH_PIN);
  int l = touchRead(L_TOUCH_PIN);
  int r = touchRead(R_TOUCH_PIN);

  curr.touched[TOP_PAD] = last.touched[TOP_PAD] ? t > T_LOW_THRESHOLD : t > T_HIGH_THRESHOLD;
  curr.touched[BOTTOM_PAD] = last.touched[BOTTOM_PAD] ? b > B_LOW_THRESHOLD : b > B_HIGH_THRESHOLD;
  curr.touched[LEFT_PAD] = last.touched[LEFT_PAD] ? l > L_LOW_THRESHOLD : l > L_HIGH_THRESHOLD;
  curr.touched[RIGHT_PAD] = last.touched[RIGHT_PAD] ? r > R_LOW_THRESHOLD : r > R_HIGH_THRESHOLD;

  // calculate lastup and lastdown times
  for (int i = 0; i < PAD_COUNT; i++) {
    if (down(i)) {
      lastDownT[i] = curr.time;
    }

    if (up(i)) {
      lastUpT[i] = curr.time;
    }
  }

  // save the last down coordinates
  if (down(SCREEN_PAD)) {
    lastDownXPos = curr.x;
    lastDownYPos = curr.y;
  }
}

bool TouchPad::touched(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (touched(i))
        return true;
      else
        return false;
    }
  }

  return curr.touched[pad];

}

bool TouchPad::changed(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (changed(i))
        return true;
      else
        return false;
    }
  }

  return curr.touched[pad] != last.touched[pad];

}

bool TouchPad::down(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (down(pad))
        return true;
      else
        return false;
    }
  }

  return changed(pad) && touched(pad);
}

bool TouchPad::up(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (up(pad))
        return true;
      else
        return false;
    }
  }

  return changed(pad) && !touched(pad);

}

long TouchPad::lastDownTime(int pad) {
  if (pad == ANY_PAD) {
    long lastest = 0;
    for (int i = 0; i < PAD_COUNT; i++) {
      if (lastDownT[i] > lastest) {
        lastest = lastDownT[i];
      }
    }
    return lastest;
  } else {
    return lastDownT[pad];
  }
}

long TouchPad::lastUpTime(int pad) {
  if (pad == ANY_PAD) {
    long lastest = 0;
    for (int i = 0; i < PAD_COUNT; i++) {
      if (lastUpT[i] > lastest) {
        lastest = lastUpT[i];
      }
    }
    return lastest;
  } else {
    return lastUpT[pad];
  }
}

int TouchPad::lastDownX(void) {
  return lastDownXPos;
}

int TouchPad::lastDownY(void) {
  return lastDownYPos;
}
