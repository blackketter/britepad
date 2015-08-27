#ifndef _BubblesApp_
#define _BubblesApp_

#include "BritepadApp.h"
#include "Britepad.h"

class BubblesApp : public BritepadApp {
  public:
    BubblesApp(void) : BritepadApp() {};
    BritepadApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Bubbles"; };
  private:
    int currentColor = screen.red;

};

#endif
