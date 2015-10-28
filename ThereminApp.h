#ifndef _ThereminApp_
#define _ThereminApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class ThereminApp : public BritepadApp {
  public:
    const char* name() { return "Theremin"; };
    void run();
    void end(BritepadApp* nextApp);
    virtual bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "thrm";
  private:
    uint8_t lastProx;
    color_t currColor = 0;
};

#endif


