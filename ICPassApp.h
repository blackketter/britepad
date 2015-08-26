#ifndef _ICPassApp_
#define _ICPassApp_

#include "BPApp.h"
#include "Britepad.h"

class ICPassApp : public PassApp {
  private:
    const char* pass(void) { return "Bu77cracks!\n"; };

  public:
    const char* name(void) { return "iCloud"; };
};

#endif

