#ifndef _BPApp_
#define _BPApp_

#include "Britepad.h"
#include "Debug.h"

class BPApp {
  public:
    BPApp() {};
    virtual void begin(void) { screen.fillScreen(screen.black); };
    virtual void end(void) {};
    virtual void run(void) {};
    virtual const char* name(void) { return 0; };
    virtual bool isScreensaver(void) { return false; };
};

#endif


