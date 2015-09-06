#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  private:
    bool enabled = true;

  public:
    virtual bool isScreensaver(void) { return true; };
    virtual color_t buttonColor(void) { return (enabled ? screen.yellow : screen.darkyellow); };
    virtual bool screensaverIsEnabled(void) { return enabled; }
    virtual void setEnabled(bool e) { enabled = e; }
};

#endif

