#ifndef _Britepad_
#define _Britepad_

#include "BritepadApp.h"

class BritepadApp;
class LauncherApp;
class ScreensaverApp;

class Britepad {
  public:
    Britepad(void);
    void addApp(BritepadApp* newApp);
    int appsAdded(void) { return appCount; }
    BritepadApp* getApp(int appIndex);

    void setApp(BritepadApp* newApp);  // sets the current app
    void begin(void);
    void idle(void);

    BritepadApp* wantsToBeScreensaver(void);

    ScreensaverApp* randomScreensaver(void);

  private:
    int appCount = 0;
    static const int maxApps = 100;
    static const millis_t screensaverDelay = 10000;
    static const millis_t screensaverSwitchInterval = 60000;

    millis_t screensaverStartedTime;

    BritepadApp* apps[maxApps];

    LauncherApp* launcherApp;
    BritepadApp* defaultApp;
    BritepadApp* currApp;
    ScreensaverApp* currScreensaver;
};

#endif
