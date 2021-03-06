#ifndef _TimeoutApp_
#define _TimeoutApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

class TimeoutApp : public BritepadApp {

  public:

    void run() {
      time_t timeout;
      timeout = launcher.getScreensaverSwitchInterval();
      switch (timeout) {
        case 0:
          timeout = 5;
          break;
        case 5:
          timeout = 10;
          break;
        case 10:
          timeout = 30;
          break;
        case 30:
          timeout = 60;
          break;
        case 60:
          timeout = 60*5;
          break;
        case 60*5:
          timeout = 60*30;
          break;
        case 60*30:
          timeout = 60*60;
          break;
        default:
          timeout = 0;
          break;
      }
      launcher.setScreensaverSwitchInterval(timeout);
    }

    const char* name() {
      time_t switchInterval = launcher.getScreensaverSwitchInterval();
      if (switchInterval == 0) {
        return "Don't Auto Switch";
      }

      if (switchInterval <= 60) {
        sprintf(nameStr, "Switch %d seconds", (int)switchInterval);
      } else {
        sprintf(nameStr, "Switch %d minutes", (int)switchInterval/60);
      }
      return nameStr;
    };

    bool canBeInvisible() { return true; };

    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "tout";

    char nameStr[30];
};

#endif

