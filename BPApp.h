#ifndef _BPApp_
#define _BPApp_

#include "Britepad.h"
#include "Debug.h"

#define DEFAULT_APP ((BPApp*)1)
#define BACK_APP ((BPApp*)2)

class BPApp {
  public:
    BPApp() {};
    virtual void begin(void) { screen.fillScreen(screen.black); };
    virtual void end(void) {};
    virtual BPApp* run(void) {return nil;};
    virtual const char* name(void) { return 0; };
    virtual bool isScreensaver(void) { return false; };
};

#endif


