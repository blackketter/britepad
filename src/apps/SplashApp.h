#ifndef _SplashApp_
#define _SplashApp_

#include "ScreensaverApp.h"

class SplashApp : public BritepadApp {
  public:
    void begin();
    void run();
    const char* name() { return "Splash"; };
    bool displaysStatusBar() { return false; };
    bool getEnabled(AppMode asMode) { return false; }  // this only runs at boot time, explicitly
    appid_t id() { return ID; };
    static constexpr appid_t ID = "spla";
    bool disablesScreensavers() override { return true; }
    BritepadApp* exitsTo() override { return A_SCREENSAVER_APP; }

  private:
    int32_t drawindex = 0;
    color_t currColor;
    millis_t firstRun;
    static const millis_t splashDuration = 1000*5;
};

#endif
