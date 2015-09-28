#ifndef _SETCLOCKAPP_
#define _SETCLOCKAPP_

#include "BritepadApp.h"
#include "BritepadShared.h"

class SetClockApp : public BritepadApp {
  public:
    void begin(AppMode asMode);
    void end(BritepadApp* nextApp);
    BritepadApp* run(void);
    const char* name(void) { return "Set Clock"; };

    bool disablesScreensavers(void) { return true; };
    bool wantsToBeScreensaver(void) { return !hasRun && !clock.hasBeenSet(); };
    bool displaysClock(void) { return true; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "sclk";

  private:
    static const int buttoncount = 7;
    static const int buttonradius = 24;
    static const int nobutton = -1;

    void drawClock(void);
    void drawButton(int i, color_t color);
    int hitButton(int x, int y);
    void drawButtons(void);
    time_t lastTime = 0;
    bool hasRun = false;

};
#endif


