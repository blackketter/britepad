#ifndef _MousePad_
#define _MousePad_

#include "BritepadShared.h"

class MousePad {
  public:
    MousePad(void) {  Mouse.begin(); };
    void begin();
    void run();
    void end();

  private:
    bool scrollMode = false;
    bool mouseLock = false;
};

#endif
