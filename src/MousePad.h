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
    uint16_t screen_h = 2560;
    uint16_t screen_v = 1440;
    uint16_t absMargin = 5; // 5% at each edge
    bool isMac = false;
};

#endif
