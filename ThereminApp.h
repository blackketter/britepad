#ifndef _ThereminApp_
#define _ThereminApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class ThereminApp : public BritepadApp {
  public:
    const char* name(void) { return "Theremin"; };
    BritepadApp* run(void);
    void end(BritepadApp* nextApp);
    virtual bool disablesScreensavers(void) { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "thrm";
  private:
    uint8_t lastProx;
    color_t currColor = 0;
};

#endif


