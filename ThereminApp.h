#ifndef _ThereminApp_
#define _ThereminApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class ThereminApp : public BritepadApp {
  public:
    ThereminApp(void) {};
    const char* name(void) { return "Theremin"; };
    BritepadApp* run(void);
    void end(BritepadApp* nextApp);
  private:
};

#endif


