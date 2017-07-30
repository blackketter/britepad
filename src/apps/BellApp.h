#ifndef _BellApp_
#define _BellApp_

#include "BritepadApp.h"
#include "widgets/Button.h"

class BellApp : public BritepadApp {

  public:
    void begin(AppMode asMode);
    void run();
    void init();

    const char* name() { return name_str; };

    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "bell";
    AppType getAppType() { return SETTINGS_APP; }

    void bellCallback();

  private:

    const char* name_str = "Bell";

    int16_t _bell_interval = 20*60;  // 20 minutes
    bool _bell_enabled = true;

    void drawTime();
    void drawButtons();
    void resetTimer();

    static const int buttoncount = 5;
    time_t _last_run;

    CallbackTimer _bell_timer;

    RoundButton button[buttoncount];
};


#endif

