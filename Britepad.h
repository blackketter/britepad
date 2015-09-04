#ifndef _Britepad_
#define _Britepad_

#include "Britepad.h"

class BritepadApp;

class Britepad {
  public:
    Britepad(void);
    void addApp(BritepadApp* newApp);
    int appsAdded(void) { return appCount; }
    BritepadApp* getApp(int appIndex);

  private:
    int appCount = 0;
    static const int maxApps = 100;
    BritepadApp* apps[maxApps];
};

#endif
