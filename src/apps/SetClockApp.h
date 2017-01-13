#ifndef _SetClockApp_
#define _SetClockApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "widgets/Button.h"
#include "Clock.h"
#include "ChimeApp.h"

class SetClockApp : public BritepadApp {
  public:
    void init() { theTime = &clock; };

    void begin(AppMode asMode);
    void end();
    void run();

    bool disablesScreensavers() { return true; };
    bool displaysClock() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    const char* name() { return "Set Clock"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "sclk";


  protected:
    Time* theTime;

    virtual color_t modeButtonColor() { return screen.red; };
    virtual font_t modeButtonFont() { return Arial_12_Bold; };
    virtual const char* modeButtonText() { return setDate ? "Time" : "Date"; };
    virtual void modeButtonPressed();

    virtual void adjustSeconds(stime_t delta) { theTime->adjustSeconds(delta); };

    static const int buttoncount = 8;
    static const int buttonradius = 24;
    static const int nobutton = -1;

    RoundButton button[buttoncount];
    void drawTime();
    void drawButtons();
    void resetSecs() { adjustSeconds(-theTime->getSeconds()%60); };
    millis_t lastDraw = 0;
    bool hasRun = false;
    bool setDate = false;

};

#endif


