#ifndef _Britepad_
#define _Britepad_

#include "BritepadShared.h"
#include "Timer.h"

class BritepadApp;
class LauncherApp;
class ScreensaverApp;

class Britepad {
  public:
    Britepad(void);
    void addApp(BritepadApp* newApp);
    int appsAdded(void) { return appCount; };
    BritepadApp* getApp(int appIndex);
    BritepadApp* getApp(appid_t appID);

    void begin(void);
    void idle(void);

    ScreensaverApp* wantsToBeScreensaver(void);

    ScreensaverApp* randomScreensaver(void);

  private:
    void setApp(BritepadApp* newApp);  // sets the current app

    int appCount = 0;
    static const int maxApps = 100;
    static const millis_t screensaverDelay = 5000;
    static const millis_t screensaverSwitchInterval = 10000;
    static const millis_t ambientUpdateInterval = 100;

    Timer     backlightTimer;

    millis_t screensaverStartedTime = 0;

    BritepadApp* apps[maxApps];

    BritepadApp* defaultApp;
    BritepadApp* currApp;
    BritepadApp* switchApp;
};

#endif
