#ifndef _TimeoutApp_
#define _TimeoutApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

class TimeoutApp : public BritepadApp {

  public:

    void run() {
      time_t timeout;
      timeout = britepad.getScreensaverSwitchInterval();
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
      britepad.setScreensaverSwitchInterval(timeout);
    }

    const char* name() {
      time_t switchInterval = britepad.getScreensaverSwitchInterval();
      if (switchInterval == 0) {
        return "Don't Auto\nSwitch";
      }

      if (switchInterval < 60) {
        sprintf(nameStr, "Switch\n%d seconds", (int)switchInterval);
      } else {
        sprintf(nameStr, "Switch\n%d minutes", (int)switchInterval/60);
      }
      return nameStr;
    };

    bool isPopup() { return true; };
    bool isInvisible() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    color_t buttonColor() { return screen.blue; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "tout";

    char nameStr[30];
};

#endif

