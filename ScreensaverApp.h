#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  public:
    virtual bool isScreensaver(void) { return true; };
    virtual color_t buttonColor(void) { return (getScreensaverEnabled() ? screen.yellow : screen.darkyellow); };
    virtual bool getScreensaverEnabled(void) { return enableScreensaver; }
    virtual void setScreensaverEnabled(bool e) { enableScreensaver = e; }

  protected:
    bool enableScreensaver = true;
};

#endif

