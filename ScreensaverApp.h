#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  public:
    virtual bool isScreensaver(void) { return true; };
    virtual color_t buttonColor(void) { return (screensaverIsEnabled() ? screen.yellow : screen.darkyellow); };
    virtual bool screensaverIsEnabled(void) { return enabled; }
    virtual void setEnabled(bool e) { enabled = e; }

  protected:
    bool enabled = true;
};

#endif

