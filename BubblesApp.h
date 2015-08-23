#ifndef _BubblesApp_
#define _BubblesApp_

#include "BPApp.h"
#include "Britepad.h"

class BubblesApp : public BPApp {
  public:
    BubblesApp(void) : BPApp() {};
    void end(void);
    BPApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Bubbles"; };
  private:
    int currentColor = screen.red;

};

#endif
