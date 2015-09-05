#ifndef _Britepad_
#define _Britepad_

#include "BritepadApp.h"

class BritepadApp;
class LauncherApp;

class Britepad {
  public:
    Britepad(void);
    void addApp(BritepadApp* newApp);
    int appsAdded(void) { return appCount; }
    BritepadApp* getApp(int appIndex);

    void setApp(BritepadApp* newApp);  // sets the current app

    void begin(void);
    void idle(void);

  private:
    int appCount = 0;
    static const int maxApps = 100;
    static const long screensaverDelay = 10000;

    BritepadApp* apps[maxApps];

    LauncherApp* launcherApp;
    BritepadApp* mouseApp;
    BritepadApp* currApp;
};

#endif
