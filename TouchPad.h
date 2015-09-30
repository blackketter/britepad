#ifndef _TOUCHPAD_
#define _TOUCHPAD_

#include "Types.h"

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
    int  x;
    int  y;
    bool touched[TOTAL_SENSORS];
} TPState;

class TouchPad {

  public:
    TouchPad(int w, int h);
    void begin(void);
    void update(void);
    bool touched(int pad = SCREEN_PAD);  // is the pad being touched?
    bool changed(int pad = SCREEN_PAD);  // has the pad touch state changed since the last update?
    bool down(int pad = SCREEN_PAD);     // has the pad been touched since the last update?
    bool up(int pad = SCREEN_PAD);       // has the pad been released since the last update?
    millis_t lastDownTime(int pad = SCREEN_PAD); // last time the pad was down
    millis_t lastUpTime(int pad = SCREEN_PAD);   // last time the pad was released
    millis_t lastTouchedTime(int pad = SCREEN_PAD); // last time the pad was touched
    int  lastDownX(void);
    int  lastDownY(void);
    int  x(void) { return curr.x; };
    int  y(void) { return curr.y; };
    millis_t  time(void) { return curr.time; };
    int  deltax(void) { return (curr.x - last.x); };
    int  deltay(void) { return (curr.y - last.y); };
    uint16_t getAmbientLight(void) { return ambientLight;};

    uint8_t getProximityMax(void) { return proximityMax; };
    uint8_t getProximityDistance(void);

    int getHeight() { return width; };
    int getWidth() { return height; };

  private:
    int height;
    int width;
    TPState curr;
    TPState last;
    millis_t lastDownT[TOTAL_SENSORS];
    millis_t lastUpT[TOTAL_SENSORS];
    int  lastDownXPos;
    int  lastDownYPos;
    static const uint8_t proximityThreshold = 100;
    static const uint8_t proximityMax = 255;
    bool getProximityPresent(void);


    void initAPDS(void);
    void updateAPDS(void);
    millis_t lastAPDSupdate = 0;
    millis_t APSDupdateInterval = 100;

    uint16_t ambientLight;
    uint16_t redLight;
    uint16_t greenLight;
    uint16_t blueLight;
    uint8_t proximity;
    int gesture;
};

#endif
