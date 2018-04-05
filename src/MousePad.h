#ifndef _MousePad_
#define _MousePad_

#include "BritepadShared.h"

class MousePad {
  public:
    void begin();
    void run();
    void end();
    void setAbsolute(bool a) { absoluteMode = a; }
    bool isAbsolute() { return absoluteMode; }
  private:

    bool scrollMode = false;
    bool absoluteMode = false;

    bool pressed = false;

    millis_t lastScroll = pad.time();
    int16_t accumScroll = 0;

    const uint16_t screen_h = 2560;
    const uint16_t screen_v = 1440;
    const uint16_t absMargin = 5; // 5% at each edge

// Mouse parameters
    const uint16_t MOUSE_MAX_MOVE = 100; // move maximum number of points

    const millis_t MOUSE_TAP_DUR = 150; // a tap is less than this many millis
    const millis_t MOUSE_TAP_UP_DUR = 200;
    const millis_t MOUSE_DRAG_DUR = 300;
    const millis_t MOUSE_RELEASE_DRAG_DUR = 1000;

    const uint16_t MOUSE_TAP_MOVEMENT_MAX = 10;
    const uint16_t SCROLL_EDGE_MARGIN = 10;
    const bool isMac = false;

    const int16_t scrollFactor = 16;
    const millis_t scrollInterval = 25;

};

#endif
