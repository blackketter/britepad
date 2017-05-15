#ifndef _TouchPad_
#define _TouchPad_

#include "Types.h"
#include "Clock.h"
#include "Gesture.h"

enum Pads {
  SCREEN_PAD,
  LEFT_PAD,
  RIGHT_PAD,
  BOTTOM_PAD,
  TOP_PAD,
  TOUCH_PAD_COUNT,
  PROXIMITY_SENSOR = TOUCH_PAD_COUNT,
  SENSOR_COUNT,
  ANY_PAD = 1000,
  ANY_SENSOR = 1001
};

enum ProximityGesture {
  PROX_NONE,
  PROX_UP,
  PROX_DOWN,
  PROX_LEFT,
  PROX_RIGHT,
  PROX_NEAR,
  PROX_FAR
};

typedef struct TPState {
    millis_t time;
    coord_t  x;
    coord_t  y;
    bool touched[SENSOR_COUNT];
} TPState;

class TouchPad {

  public:
    TouchPad(coord_t w, coord_t h);
    void begin();
    void update();
    bool touched(int pad = SCREEN_PAD);  // is the pad being touched?
    bool changed(int pad = SCREEN_PAD);  // has the pad touch state changed since the last update?
    bool pressed(int pad = SCREEN_PAD);     // has the pad been touched since the last update?
    bool released(int pad = SCREEN_PAD);       // has the pad been released since the last update?
    millis_t lastDownTime(int pad = SCREEN_PAD); // last time the pad was down
    millis_t lastUpTime(int pad = SCREEN_PAD);   // last time the pad was released
    millis_t lastTouchedTime(int pad = SCREEN_PAD); // last time the pad was touched
    millis_t holdTime(int pad = SCREEN_PAD) { return time() - lastDownTime(pad); }  // utility for how much time it's been held down
    coord_t  lastDownX();
    coord_t  lastDownY();
    coord_t  x() { return curr.x; };
    coord_t  y() { return curr.y; };
    millis_t  time() { return curr.time; };
    coord_t  deltax() { return (curr.x - last.x); };
    coord_t  deltay() { return (curr.y - last.y); };
    uint16_t getAmbientLight() { return ambientLight;};

    uint8_t getProximityMax() { return proximityMax; };
    uint8_t getProximityDistance();
    uint8_t getProximityRaw() { return proximity; };
    ProximityGesture getProximityGesture();

    coord_t getHeight() { return width; };
    coord_t getWidth() { return height; };

    bool didGesture();
    bool isGesturing();
    gesture_t getGesture(const gestureData_t* gestureList = defaultGestures);
    angle8_t getGestureOrientation() { return lastGestureOrientation; }
    uint16_t getGestureDistance() { return lastGestureDistance; }

    int getHistoryCount() { return historyCount; }
    point_t* getHistory() { return history; }

  private:
    coord_t height;
    coord_t width;
    TPState curr;
    TPState last;
    millis_t lastDownT[SENSOR_COUNT];
    millis_t lastUpT[SENSOR_COUNT];
    coord_t  lastDownXPos;
    coord_t  lastDownYPos;
    static const uint8_t proximityThreshold = 125;
    static const uint8_t proximityMax = 255;
    static const uint16_t ambientMax = 255;
    bool getProximityPresent();
    bool ctpWorking = false;


    static const int maxHistory = 256;
    static const millis_t minHistoryInterval = 5;
    millis_t lastHistoryTime;
    point_t history[maxHistory];
    coord_t historyMaxX, historyMaxY, historyMinX, historyMinY;
    int historyCount = 0;

    gesture_t lastGesture = Gesture::NO_GESTURE;
    angle8_t lastGestureOrientation = 0;
    uint16_t lastGestureDistance = 0;
    const gestureData_t* gesturesSearched;

    void initAPDS();
    void updateAPDS();
    millis_t lastAPDSupdate = 0;
    millis_t APSDupdateInterval = 10;

    uint16_t ambientLight = ambientMax;
    uint8_t proximity;
    ProximityGesture proximityGesture = PROX_NONE;

    static const uint8_t proxGestureBeginThreshold = 100;
    static const uint8_t proxGestureEndTheshold = 90;
};

extern TouchPad pad;

#endif
