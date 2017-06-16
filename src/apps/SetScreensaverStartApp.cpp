#ifndef _ScreensaverStartApp_
#define _ScreensaverStartApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

class ScreensaverStartApp : public BritepadApp {

  public:

    void run() {
      time_t timeout;
      timeout = britepad.getScreensaverStartInterval();
      switch (timeout) {
        case 3:
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
          timeout = 3;
          break;
      }
      britepad.setScreensaverStartInterval(timeout);
    }

    const char* name() {
      time_t startInterval = britepad.getScreensaverStartInterval();
      if (startInterval == 0) {
        return "No screensaver";
      } else {
        sprintf(nameStr, "Start %d seconds", (int)startInterval);
      }
      return nameStr;
    };

    bool canBeInvisible() { return true; };

    AppType getAppType() { return SETTINGS_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "ssst";

    char nameStr[30];
};

ScreensaverStartApp theScreensaverStartApp;

#endif

