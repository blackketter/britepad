#ifndef _LifeApp_
#define _LifeApp_

#include "DotsDisplayApp.h"

class LifeApp : public DotsDisplayApp {
  public:
    void run();

    void switchAppMode(AppMode asMode);

    bool canBeScreensaver() { return true; }
    bool canBeInteractive() { return true; }
    bool canBeMouse() { return true; }

    const char* name() { return "Life"; };
    static constexpr appid_t ID = "life";
    appid_t id() { return ID; };

  protected:
    int getDotsWide() { return 32; }
    int getDotsHigh() { return 24; }

  private:
    void seed();
    void wipe();
    void iterate();
    int32_t generation;
    int32_t lastpopulation = 0;
    int32_t samelastpopulation = 0;
    millis_t nextRun = 0;
    bool reseed = true;
    int ruleset = 0;
};

#endif


