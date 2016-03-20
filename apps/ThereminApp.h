#ifndef _ThereminApp_
#define _ThereminApp_

#include "ScreensaverApp.h"
#include "BritepadShared.h"

class ThereminApp : public BritepadApp {
  public:
    void run();
    void end();
    virtual bool disablesScreensavers() { return true; }

    const char* name() { return "Theremin"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "thrm";

  private:
    uint8_t lastProx;
    color_t currColor = 0;
    float lastPitch = 0.0;
};

#endif


