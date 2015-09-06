#ifndef _TOUCHPAD_
#define _TOUCHPAD_

#include "Types.h"

#define SCREEN_PAD 0
#define LEFT_PAD 1
#define RIGHT_PAD 2
#define BOTTOM_PAD 3
#define TOP_PAD 4
#define PAD_COUNT 5
#define ANY_PAD 6

typedef struct TPState {
    long time;
    int  x;
    int  y;
    bool touched[PAD_COUNT];
} TPState;

class TouchPad {
  private:
    int height;
    int width;
    TPState curr;
    TPState last;
    long lastDownT[PAD_COUNT];
    long lastUpT[PAD_COUNT];
    int  lastDownXPos;
    int  lastDownYPos;

  public:
    TouchPad(int w, int h);
    void begin(void);
    void update(void);
    bool touched(int pad = 0);  // is the pad being touched?
    bool changed(int pad = 0);  // has the pad touch state changed since the last update?
    bool down(int pad = 0);     // has the pad been touched since the last update?
    bool up(int pad = 0);       // has the pad been released since the last update?
    long lastDownTime(int pad = 0); // last time the pad was touched down
    long lastUpTime(int pad = 0);   // last time the pad was released
    int  lastDownX(void);
    int  lastDownY(void);
    int  x(void) { return curr.x; };
    int  y(void) { return curr.y; };
    millis_t  time(void) { return curr.time; };
    int  deltax(void) { return (curr.x - last.x); };
    int  deltay(void) { return (curr.y - last.y); };
};

#endif
