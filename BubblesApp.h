#ifndef _BubblesApp_
#define _BubblesApp_

#include "ScreensaverApp.h"

class BubblesApp : public ScreensaverApp {
  public:
    BritepadApp* run(void);
    const char* name(void) { return "Bubbles"; };
  private:
    int currentColor = screen.red;

};

#endif
