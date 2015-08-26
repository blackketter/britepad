#ifndef _MOUSEAPP_
#define _MOUSEAPP_

#include "BPApp.h"

class MouseApp : public BPApp {
  public:
    MouseApp(void) : BPApp() {};
    BPApp* run(void);
    const char* name(void) { return "Mouse"; };
  private:
    int currentColor = screen.red;
    int backgroundColor = screen.black;
};

#endif
