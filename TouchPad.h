#ifndef _TouchPad_
#define _TouchPad_

#include "Types.h"
#include "Clock.h"
#include "Gesture.h"

// todo - turn these into a scoped enum
#define SCREEN_PAD 0
#define LEFT_PAD 1
#define RIGHT_PAD 2
#define BOTTOM_PAD 3
#define TOP_PAD 4

#define TOUCH_PAD_COUNT 5

#define PROXIMITY_SENSOR 5

#define TOTAL_SENSORS 6

#define ANY_PAD 1000

typedef struct TPState {
    millis_t time;
    coord_t  x;
    coord_t  y;
    bool touched[TOTAL_SENSORS];
} TPState;

class TouchPad {

  public:
    TouchPad(coord_t w, coord_t h);
    void begin();
    void update();
    bool touched(int pad = SCREEN_PAD);  // is the pad being touched?
    bool changed(int pad = SCREEN_PAD);  // has the pad touch state changed since the last update?
    bool down(int pad = SCREEN_PAD);     // has the pad been touched since the last update?
    bool up(int pad = SCREEN_PAD);       // has the pad been released since the last update?
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

    coord_t getHeight() { return width; };
    coord_t getWidth() { return height; };

    int getHistoryCount() { return historyCount; }
    point_t* getHistory() { return history; }

    bool didGesture();
    gesture_t getGesture(const gestureData_t* gestureList = defaultGestures);
    angle8_t getGestureOrientation() { return lastGestureOrientation; }
    uint16_t getGestureDistance() { return lastGestureDistance; }

  private:
    coord_t height;
    coord_t width;
    TPState curr;
    TPState last;
    millis_t lastDownT[TOTAL_SENSORS];
    millis_t lastUpT[TOTAL_SENSORS];
    coord_t  lastDownXPos;
    coord_t  lastDownYPos;
    static const uint8_t proximityThreshold = 115;
    static const uint8_t proximityMax = 255;
    bool getProximityPresent();

    static const int maxHistory = 256;
    static const millis_t minHistoryInterval = 5;
    millis_t lastHistoryTime;
    point_t history[maxHistory];
    int historyCount = 0;

    gesture_t lastGesture = Gesture::NO_GESTURE;
    angle8_t lastGestureOrientation = 0;
    uint16_t lastGestureDistance = 0;
    const gestureData_t* gesturesSearched;

    void initAPDS();
    void updateAPDS();
    millis_t lastAPDSupdate = 0;
    millis_t APSDupdateInterval = 100;

    uint16_t ambientLight;
    uint8_t proximity;

};

extern TouchPad pad;

#endif
