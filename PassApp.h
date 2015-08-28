#ifndef _PassApp_
#define _PassApp_

#include "BritepadApp.h"
#include "Britepad.h"

class PassApp : public BritepadApp {

  protected:
    virtual const char* pass(void) { return "ch@db1ldr3n\n"; };

  public:
    PassApp(void) : BritepadApp() {};
    BritepadApp* run(void);
    const char* name(void) { return "Pass"; };
    color_t buttonColor(void) { return screen.red; };
};

#endif
