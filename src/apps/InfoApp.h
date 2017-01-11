#ifndef _InfoApp_
#define _InfoApp_
#include "BritepadApp.h"

class InfoApp : public BritepadApp {
  public:
    void run();
    void init();
    void begin(AppMode asMode) { max = 0; BritepadApp::begin(asMode);}
    bool displaysStatusBar() { return false; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return true; }
    color_t bgColor() { return screen.blue; }

    const char* name() { return "Info"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "info";

    AppType getAppType() { return DEBUG_APP; }

  private:
    millis_t lastUpdate = 0;
    uint8_t max = 0;
};


#endif
