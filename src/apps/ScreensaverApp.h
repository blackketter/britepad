#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "BritepadShared.h"
#include "BritepadApp.h"

class ScreensaverApp : public BritepadApp {
  public:
    virtual color_t buttonColor() { return screen.yellow; };

    // by default, no status bars
    virtual bool displaysStatusBar() { return false; }

    // if the app was launched as interactive, then other screensavers shouldn't run
    virtual bool disablesScreensaver() { return isAppMode(INTERACTIVE_MODE); }

    AppType getAppType() { return (AppType)((canBeMouse() ? MOUSE_APP : NO_APP_TYPE) | SCREENSAVER_APP); }

    // by default, screensavers are not interactive
    virtual bool canBeScreensaver() { return  clock.hasBeenSet() || !displaysClock(); }
    virtual bool canBeInteractive() { return false; }

  protected:
};

#endif

