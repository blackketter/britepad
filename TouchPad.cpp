#include "Arduino.h"

#include <Adafruit_FT6206.h>
#include <SparkFun_APDS9960.h>

#include "TouchPad.h"

#define DEBUG_ON 1
#include "Debug.h"

#define B_TOUCH_PIN A2
#define T_TOUCH_PIN A8
#define L_TOUCH_PIN A1
#define R_TOUCH_PIN A3

#define APDS9960_INT    20  // Needs to be an interrupt pin


#define L_HIGH_THRESHOLD 950
#define L_LOW_THRESHOLD 850

#define R_HIGH_THRESHOLD 1250
#define R_LOW_THRESHOLD 1150

#define B_HIGH_THRESHOLD 1100
#define B_LOW_THRESHOLD 1000

#define T_HIGH_THRESHOLD 1150
#define T_LOW_THRESHOLD 1050

Adafruit_FT6206 ctp = Adafruit_FT6206();
SparkFun_APDS9960 apds = SparkFun_APDS9960();


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
  initAPDS();

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
  updateAPDS();
 }

void TouchPad::updateAPDS(void) {

  if (curr.time / 1000 != lastAPDSupdate/1000) {
    lastAPDSupdate = curr.time;

    //  update the APDS9960
    uint16_t light;
    if (apds.readAmbientLight(light)) {
      ambientLight = light;
      DEBUG_PARAM_LN("ambientLight", light);
    } else {
      ambientLight = 0;
      DEBUG_LN("error reading ambient light");
    }

    if (apds.readRedLight(light)) {
      redLight = light;
      DEBUG_PARAM_LN("red light", light);
    } else {
      redLight = 0;
      DEBUG_LN("error reading red light");
    }

    if (apds.readGreenLight(light)) {
      greenLight = light;
     DEBUG_PARAM_LN("green light", light);
    } else {
      greenLight = 0;
      DEBUG_LN("error reading green light");
    }

    if (apds.readBlueLight(light)) {
      DEBUG_PARAM_LN("blue light", light);
      blueLight = light;
    } else {
      blueLight = 0;
      DEBUG_LN("error reading blue light");
    }

    uint8_t prox;
    if (apds.readProximity(prox)) {
      DEBUG_PARAM_LN("proximity light", light);
      proximity = prox;
    } else {
      proximity = 0;
      DEBUG_LN("error reading proximity");
    }

    if (apds.isGestureAvailable()) {
      gesture = apds.readGesture();
    } else {
      gesture = 0;
    }
  }
}

void TouchPad::initAPDS(void) {

// initialize APDS9960 interrupt
  bool useAPDS9960Interrupts = false;
  if (useAPDS9960Interrupts) {
    //pinMode(APDS9960_INT, INPUT);
    //attachInterrupt(0, interruptRoutine, FALLING);
  }

  if (apds.init()) {
    DEBUG_LN("APDS-9960 Initialized");
    apds.enableLightSensor(useAPDS9960Interrupts);
    apds.enableProximitySensor(useAPDS9960Interrupts);

    // gestures lock up device.  I suspect that they require interrupts
    // apds.enableGestureSensor(useAPDS9960Interrupts);
  } else {
    DEBUG_LN("APDS-9960 Fail init()");
  }
}

bool TouchPad::touched(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (touched(i))
        return true;
    }
    return false;
  }

  return curr.touched[pad];

}

bool TouchPad::changed(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (changed(i))
        return true;
    }
    return false;
  }

  return curr.touched[pad] != last.touched[pad];

}

bool TouchPad::down(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (down(i))
        return true;
    }
    return false;
  }

  return changed(pad) && touched(pad);
}

bool TouchPad::up(int pad) {
  if (pad == ANY_PAD) {
    for (int i = 0; i < PAD_COUNT; i++) {
      if (up(pad))
        return true;
    }
    return false;
  }

  return changed(pad) && !touched(pad);

}

millis_t TouchPad::lastDownTime(int pad) {
  if (pad == ANY_PAD) {
    millis_t lastest = 0;
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

millis_t TouchPad::lastUpTime(int pad) {
  if (pad == ANY_PAD) {
    millis_t lastest = 0;
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
