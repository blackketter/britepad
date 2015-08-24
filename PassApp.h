#ifndef _PassApp_
#define _PassApp_

#include "BPApp.h"
#include "Britepad.h"

class PassApp : public BPApp {

  protected:
    virtual const char* pass(void) { return "ch@db1ldr3n\n"; };

  public:
    PassApp(void) : BPApp() {};
    void begin(void);
    BPApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return "Pass"; };
};

#endif
