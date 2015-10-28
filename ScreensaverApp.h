#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  public:
    virtual color_t buttonColor() { return (getEnabled() ? screen.yellow : screen.darkyellow); };

    // by default, no status bars
    virtual bool displaysStatusBar() { return false; }

    // if the app was launched as interactive, then other screensavers shouldn't run
    virtual bool disablesScreensaver() { return isAppMode(INTERACTIVE); }

    // by default, screensavers are not interactive
    virtual bool canBeScreensaver() { return true; }
    virtual bool canBeInteractive() { return false; }

    // by default, screensavers just have one setting for if they are enabled or not
    virtual bool getEnabled() { readPrefs(); return enabled; }
    virtual void setEnabled(bool e) { enabled = e; writePrefs(); }

    virtual void writePrefs() {  prefs.write(id(), sizeof(enabled), (uint8_t*)&enabled); };
    virtual void readPrefs() { prefs.read(id(),  sizeof(enabled), (uint8_t*)&enabled); };
  protected:
};

#endif

