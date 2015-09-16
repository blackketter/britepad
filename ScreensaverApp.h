#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  public:
    virtual bool isScreensaver(void) { return true; };
    virtual color_t buttonColor(void) { return (getScreensaverEnabled() ? screen.yellow : screen.darkyellow); };
    virtual bool getScreensaverEnabled(void) { readPrefs(); return enableScreensaver; }
    virtual void setScreensaverEnabled(bool e) { enableScreensaver = e; writePrefs(); }
    virtual void writePrefs(void) {  prefs.write(id(), sizeof(enableScreensaver), (uint8_t*)&enableScreensaver); };
    virtual void readPrefs(void) { prefs.read(id(),  sizeof(enableScreensaver), (uint8_t*)&enableScreensaver); };
  protected:
    uint8_t enableScreensaver = true;
};

#endif

