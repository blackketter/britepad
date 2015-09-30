#ifndef _LifeApp_
#define _LifeApp_

#include "DotsDisplayApp.h"

class LifeApp : public DotsDisplayApp {
  public:
    void begin(AppMode asMode);
    BritepadApp* run(void);
    void end(void);

    bool canBeAppMode(AppMode b) { return true; }

    void setAppMode( AppMode newMode );

    const char* name(void) { return "Life"; };
    static constexpr appid_t ID = "life";
    appid_t id() { return ID; };

  private:
    void seed();
    void wipe();
    void iterate();
    long generation;
    millis_t nextRun = 0;
    bool reseed = true;
};

#endif


