#ifndef _RebootApp_
#define _RebootApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"
#include "Commands/TeensyCommands.h"

class RebootApp : public BritepadApp {

  public:

    void run() {
      reboot();
    }

    const char* name() { return "Reboot"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "boot";

    AppType getAppType() { return DEBUG_APP; }


};

class ResetApp : public RebootApp {
  public:

    void run() {
      prefs.reset();
      // reset the clock to 0
      clock.setSeconds(0);
      RebootApp::run();
    }

    const char* name() { return "Reset"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "rset";

};


class ReloadApp : public RebootApp {
  public:

    void run() {
      reload();
    }

    const char* name() { return "Reload"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "rlod";
    AppType getAppType() { return (AppType)(DEBUG_APP + KEY_APP); }

    EventPriority eventPriority() { return PRIORITY_LAST; }
    bool event(Event* k) {
      if (k->pressed(KEY_ENTER) && events->keyIsDown(KEY_P) && events->keyIsDown(KEY_U)) {
        launch();
      }
      return false;
    };

};
#endif


