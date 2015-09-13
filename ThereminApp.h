#ifndef _ThereminApp_
#define _ThereminApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class ThereminApp : public BritepadApp {
  public:
    const char* name(void) { return "Theremin"; };
    BritepadApp* run(void);
    void end(BritepadApp* nextApp);

    appid_t id() { return ID; };
    static constexpr appid_t ID = "thrm";
};

#endif


