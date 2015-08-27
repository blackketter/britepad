#ifndef _MOUSEAPP_
#define _MOUSEAPP_

#include "BritepadApp.h"

class MouseApp : public BritepadApp {
  public:
    MouseApp(void) : BritepadApp() {};
    BritepadApp* run(void);
    const char* name(void) { return "Mouse"; };
  private:
    int currentColor = screen.red;
    int backgroundColor = screen.black;
};

#endif
