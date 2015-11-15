#ifndef _SetClockApp_
#define _SetClockApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Button.h"
#include "Clock.h"

class SetClockApp : public BritepadApp {
  public:
    SetClockApp() { theTime = &clock; };

    void begin();
    void end();
    void run();
    bool disablesScreensavers() { return true; };
    bool wantsToBeScreensaver() { return !hasRun && !clock.hasBeenSet(); };
    bool displaysClock() { return true; };

    const char* name() { return "Set Clock"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "sclk";


  protected:
    Time* theTime;

    virtual color_t modeButtonColor() { return screen.red; };
    virtual font_t modeButtonFont() { return Arial_12_Bold; };
    virtual const char* modeButtonText() { return setDate ? "Time" : "Date"; };
    virtual void modeButtonPressed();

    virtual void adjust(stime_t delta) { theTime->adjust(delta); };

  private:
    static const int buttoncount = 8;
    static const int buttonradius = 24;
    static const int nobutton = -1;

    RoundButton button[buttoncount];
    void drawTime();
    void drawButtons();
    void resetSecs() { adjust(-theTime->get()%60); };
    millis_t lastDraw = 0;
    bool hasRun = false;
    bool setDate = false;

};
#endif


