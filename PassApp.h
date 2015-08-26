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
    const char* name(void) { return "Pass"; };
    color_t buttonColor(void) { return screen.red; };
};

#endif
