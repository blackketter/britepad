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

    void begin(void);
    void idle(void);

    void addApp(BritepadApp* newApp);
    int appsAdded(void) { return appCount; };
    BritepadApp* getApp(int appIndex);
    BritepadApp* getApp(appid_t appID);
    BritepadApp* currentApp() { return currApp; }
    void updateStatusBar(void);

  private:
    void setApp(BritepadApp* newApp, bool asScreensaver);  // sets the current app

    ScreensaverApp* wantsToBeScreensaver(void);
    ScreensaverApp* randomScreensaver(void);


    int appCount = 0;
    static const int maxApps = 100;
    static const millis_t screensaverDelay = 5000;
    static const millis_t screensaverSwitchInterval = 30000;
    static const millis_t ambientUpdateInterval = 100;

    Timer     backlightTimer;
    Timer     statusBarUpdateTimer;

    millis_t screensaverStartedTime = 0;

    BritepadApp* apps[maxApps];

    BritepadApp* currApp;
    BritepadApp* switchApp;
};

#endif
