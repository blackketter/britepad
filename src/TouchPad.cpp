#include "Arduino.h"

#include <Adafruit_FT6206.h>
#include <SparkFun_APDS9960.h>

#include "BritepadShared.h"
#include "TouchPad.h"
#include "Hardware.h"

Adafruit_FT6206 ctp = Adafruit_FT6206();
SparkFun_APDS9960 apds = SparkFun_APDS9960();

void copyTPState( TPState* dest, TPState* src ) {
  memcpy(dest,src,sizeof(TPState));
}

TouchPad::TouchPad(coord_t w, coord_t h) {

  height = h;
  width = w;

}

void TouchPad::begin() {
  console.debugln("touchpad begin");

  if (!ctp.begin(40)) {  // pass in 'sensitivity' coefficient
    console.debugln("Couldn't start FT6206 touchscreen controller");
  } else {
    console.debugln("touchpad working");
    ctpWorking = true;
  }
  console.debugln("starting ambient");
  initAPDS();
  console.debugln("started ambient");
}

void TouchPad::update() {
  // save the last state for future reference
  copyTPState(&last, &curr);

  curr.time = Uptime::millis();

  updateAPDS();
  curr.touched[PROXIMITY_SENSOR] = getProximityPresent();

  if (ctpWorking) {
    curr.touched[SCREEN_PAD] = ctp.touched();
    // Retrieve a point
    TS_Point p = ctp.getPoint();

    curr.x = p.y;
    curr.y = p.x;

    // bogus coordinates at 0,0
    if (curr.x == 0 && curr.y == 0 && curr.touched[SCREEN_PAD]) {
      console.debugln("bogus 0,0 coordinates");
      curr.touched[SCREEN_PAD] = 0;
    }

    // flip it around to match the screen.
    if (ctp.getChipID() == FT6236_CHIPID) {
      curr.x = map(curr.x, 0, height, height, 0);
    } else {
      curr.y = map(curr.y, 0, width, width, 0);
    }

  } else {
    curr.touched[SCREEN_PAD] = false;
  }

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
  for (int i = 0; i < SENSOR_COUNT; i++) {
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
    historyCount = 0;
    historyMaxX = INT16_MIN;
    historyMinX = INT16_MAX;
    historyMaxY = INT16_MIN;
    historyMinY = INT16_MAX;
  }

  if (touched(SCREEN_PAD)) {
    if (historyCount < maxHistory && ((history[historyCount].x != curr.x) || (history[historyCount].y != curr.y))){
      // touchpad seems to be a little jumpy, filter out any giant steps
      if ((historyCount == 0) ||
            ( (abs(curr.x-history[historyCount-1].x) < 128) &&
               abs(curr.y-history[historyCount-1].y < 128) &&
               ((curr.time-lastHistoryTime) >= minHistoryInterval)
          ) ) {
        history[historyCount].x = curr.x;
        history[historyCount].y = curr.y;
        if (curr.y > historyMaxY) { historyMaxY = curr.y; }
        if (curr.y < historyMinY) { historyMinY = curr.y; }
        if (curr.x > historyMaxX) { historyMaxX = curr.x; }
        if (curr.x < historyMinX) { historyMinX = curr.y; }
        historyCount++;
        lastHistoryTime = curr.time;
      }
    } else {
//      console.debugln("Exceeded history size");
    }
  }
  if (!up(SCREEN_PAD)) {
    gesturesSearched = nullptr;
    lastGesture = Gesture::NO_GESTURE;
  }
 }


// todo: save this capture and reuse in getGesture()
bool TouchPad::didGesture() {
  Gesture newGesture;
  return (up() && newGesture.capture());
}

bool TouchPad::isGesturing() {
  return touched(SCREEN_PAD) &&
  getHistoryCount() > 1 &&
  ( historyMaxX - historyMinX > Gesture::minDimension() ||
    historyMaxY - historyMinY > Gesture::minDimension() );
}

gesture_t TouchPad::getGesture(const gestureData_t* gestureList) {
  if ( gesturesSearched == gestureList || !up() ) { return lastGesture; }

  Gesture newGesture;

  if (newGesture.capture()) {
    lastGesture = newGesture.match(gestureList, &lastGestureDistance);
    lastGestureOrientation = newGesture.getOrientation();
    gesturesSearched = gestureList;
  }

  return lastGesture;
}

bool TouchPad::getProximityPresent() {
//  screen.drawLine(proximity, 239, 319,239, screen.blue);
  if (proximity > proximityThreshold) {
//    screen.drawLine(0,239,proximity,239, screen.red);
    return true;
  } else {
//    screen.drawLine(0,239,proximity,239, screen.green);
    return false;
  }
};

uint8_t TouchPad::getProximityDistance() {
  return getProximityPresent() ? (int32_t)(proximity - proximityThreshold) * proximityMax / (proximityMax -proximityThreshold) : 0;
};

void TouchPad::updateAPDS() {

  if (curr.time / APSDupdateInterval != lastAPDSupdate / APSDupdateInterval) {
    lastAPDSupdate = curr.time;

    uint8_t prox;
    if (apds.readProximity(prox)) {
      proximity = prox;
    } else {
      proximity = 0;
      console.debugln("error reading proximity");
    }

    //  update the APDS9960
    uint16_t light;
    if (apds.readAmbientLight(light)) {
      ambientLight = light;
    } else {
      ambientLight = ambientMax;
//      console.debugln("error reading ambient light");
    }

#if 0
    if (apds.readRedLight(light)) {
      redLight = light;
    } else {
      redLight = 0;
      console.debugln("error reading red light");
    }

    if (apds.readGreenLight(light)) {
      greenLight = light;
    } else {
      greenLight = 0;
      console.debugln("error reading green light");
    }

    if (apds.readBlueLight(light)) {
      blueLight = light;
    } else {
      blueLight = 0;
      console.debugln("error reading blue light");
    }
    console.debugf("Red: %d\n", redLight);
    console.debugf("Green: %d\n", greenLight);
    console.debugf("Blue: %d\n", blueLight);
#endif

    // check for proximity gesture data
    proximityGesture = PROX_NONE;

    static millis_t lastt = 0;
    static millis_t firstt = 0;
    millis_t now = Uptime::millis();
    static int totalSamples = 0;
    static millis_t upBegin = 0;
    static millis_t downBegin = 0;
    static millis_t leftBegin = 0;
    static millis_t rightBegin = 0;
    static millis_t upEnd = 0;
    static millis_t downEnd = 0;
    static millis_t leftEnd = 0;
    static millis_t rightEnd = 0;

    uint8_t up,down,left,right;
    int i = 0;
    while(apds.readGestureData(up,down,left,right)) {
//      console.debugf("u:%3d d:%3d l:%3d r:%3d\n",up,down,left,right);
      char d = 0;
      if (up > down  && up > right   && up > left) { d = 'U'; }
      if (down > up  && down > right && down > left) { d = 'D'; }
      if (left > up  && left > down  && left > right) { d = 'L'; }
      if (right > up && right > down && right > left) { d = 'R'; }
      if (d) {
      //  console.debugf("max: %c\n", d);
      }

      if (!upBegin && up > proxGestureBeginThreshold) { upBegin = now; console.debug("upBegin\n"); }
      if (!downBegin && down > proxGestureBeginThreshold) { downBegin = now; console.debug("downBegin\n");}
      if (!leftBegin && left > proxGestureBeginThreshold) { leftBegin = now; console.debug("leftBegin\n");}
      if (!rightBegin && right > proxGestureBeginThreshold) { rightBegin = now; console.debug("rightBegin\n");}

      if (upBegin && !upEnd && up < proxGestureEndTheshold) { upEnd = now; console.debug("upEnd\n");}
      if (downBegin && !downEnd && down < proxGestureEndTheshold) { downEnd = now; console.debug("downEnd\n");}
      if (leftBegin && !leftEnd && left < proxGestureEndTheshold) { leftEnd = now; console.debug("leftEnd\n");}
      if (rightBegin && !rightEnd && right < proxGestureEndTheshold) { rightEnd = now; console.debug("rightEnd\n");}
      i++;
    }

    millis_t gestureQuietTime = 20;

    if (i) {
      if (lastt == 0) {
        firstt = now;
//      console.debugf("firstt: %d\n",firstt);
      }
      lastt = now;
//      console.debugf("lastt: %d\n",lastt);
      totalSamples += i;
//      console.debugf("prox gesture sample count: %d\n",i);
    } else if ((totalSamples != 0) && ((now - lastt) > gestureQuietTime)) {
      //console.debugf("  gesture end detected, samples: %d\n", totalSamples);

//      millis_t gestureDur = lastt - firstt;
      //console.debugf("  gesture duration: %dms\n", gestureDur);

      upBegin -= firstt;
      downBegin -= firstt;
      leftBegin -= firstt;
      rightBegin -= firstt;
      //console.debugf("  start times: u:%3d d:%3d l:%3d r:%3d\n", upBegin, downBegin,leftBegin,rightBegin);

      upEnd -= firstt;
      downEnd -= firstt;
      leftEnd -= firstt;
      rightEnd -= firstt;
      //console.debugf("  delta times: u:%3d d:%3d l:%3d r:%3d\n", upEnd-upBegin, downEnd-downBegin,leftEnd-leftBegin,rightEnd-rightBegin);
      totalSamples = 0;
      upBegin = downBegin = rightBegin = leftBegin = upEnd = downEnd = leftEnd = rightEnd = 0;
      firstt = lastt = 0;
    }
  }
}

void TouchPad::initAPDS() {

  // not using APDS9960 interrupt
  bool useAPDS9960Interrupts = false;
  if (useAPDS9960Interrupts) {
    //pinMode(APDS9960_INT, INPUT);
    //attachInterrupt(0, interruptRoutine, FALLING);
  }

  if (apds.init()) {
    console.debugln("APDS-9960 Initialized");
    apds.enableLightSensor(useAPDS9960Interrupts);
    apds.enableProximitySensor(useAPDS9960Interrupts);
    apds.setGestureEnterThresh(proxGestureBeginThreshold); // default is 40
    apds.setGestureExitThresh(proxGestureEndTheshold); // default is 30
    apds.setLEDDrive(LED_DRIVE_25MA); // default is 100
    apds.setGestureGain(GGAIN_1X); // default is 4x
    apds.enableGestureSensor(useAPDS9960Interrupts);
    console.debugln("APDS-9960 Configured");

  } else {
    console.debugln("APDS-9960 Fail init()");
  }
}

bool TouchPad::touched(int pad) {
  if (pad >= ANY_PAD) {
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
      if (touched(i))
        return true;
    }
    return false;
  }

  return curr.touched[pad];

}

bool TouchPad::changed(int pad) {
  if (pad >= ANY_PAD) {
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
      if (changed(i))
        return true;
    }
    return false;
  }

  return curr.touched[pad] != last.touched[pad];

}

bool TouchPad::down(int pad) {
  if (pad >= ANY_PAD) {
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
      if (down(i))
        return true;
    }
    return false;
  }

  return changed(pad) && touched(pad);
}

bool TouchPad::up(int pad) {
  if (pad >= ANY_PAD) {
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
      if (up(pad))
        return true;
    }
    return false;
  }

  return changed(pad) && !touched(pad);

}

millis_t TouchPad::lastDownTime(int pad) {
  if (pad >= ANY_PAD) {
    millis_t lastest = 0;
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
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
  if (pad >= ANY_PAD) {
    millis_t lastest = 0;
    for (int i = 0; i < (pad == ANY_PAD ? TOUCH_PAD_COUNT : SENSOR_COUNT); i++) {
      if (lastUpT[i] > lastest) {
        lastest = lastUpT[i];
      }
    }
    return lastest;
  } else {
    return lastUpT[pad];
  }
}

millis_t TouchPad::lastTouchedTime(int pad) {
  return touched(pad) ? time() : lastUpTime(pad);
}

coord_t TouchPad::lastDownX() {
  return lastDownXPos;
}

coord_t TouchPad::lastDownY() {
  return lastDownYPos;
}

ProximityGesture TouchPad::getProximityGesture() {
    return proximityGesture;
}

