#ifndef _RebootApp_
#define _RebootApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

void reboot() {
      while (1) {
        delay(100);
        CPU_RESTART
      };
}

class RebootCommand : public Command {
  public:
    const char* getName() { return "reboot"; }
    const char* getHelp() { return "Reboots system"; }
    void execute(Stream* c, uint8_t paramCount, char** params) { reboot(); }
};

RebootCommand theRebootCommand;

class RebootApp : public BritepadApp {

  public:

    void run() {
      reboot();
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


