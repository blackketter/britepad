#ifndef _RebootApp_
#define _RebootApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

class RebootApp : public BritepadApp {

  public:

    RebootApp() {};

    void run() {

      while (1) {
        delay(100);
        CPU_RESTART
      };
    }

    const char* name() { return "Reboot"; };

    bool isInvisible() { return true; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "boot";

    AppType getAppType() { return DEBUG_APP; }

};

class ResetApp : public RebootApp {
  public:

    void run() {
      prefs.resetPrefs();
      // reset the clock to 0
      clock.setSeconds(0);
      RebootApp::run();
    }

    const char* name() { return "Reset"; };

    bool isInvisible() { return true; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "rset";

};
#endif


