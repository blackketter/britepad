#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "Debug.h"

// todo move this into the base object
#define BritepadAppScratchPadSize (16768L)
extern uint8_t BritepadAppScratchPad[];

class Britepad;

class BritepadApp {
  public:
    BritepadApp();
    virtual void begin(AppMode asMode);  // initialize app state and draw first screen
    virtual void end(BritepadApp* nextApp); // called after final run(), lets app clean up and tells it what the next app may be
    virtual BritepadApp* run(void) { if (isAppMode(MOUSE)) { mouse.run(); } return STAY_IN_APP;};  // run current app state repeatedly, returns pointer to next app to run (or one of the constants below)

    virtual const char* name(void) = 0;
    virtual appid_t id(void) = 0;
    bool isID(appid_t match);

    virtual color_t buttonColor(void) { return screen.blue; }

    virtual bool isCurrentApp(void) { return britepad.currentApp() == this; };

    virtual bool disablesScreensavers(void) { return false; }
    virtual bool wantsToBeScreensaver(void) { return false; }  // return true if you want to be switched to as the screensaver (canBeScreensaver() doesn't have to be true)
    virtual BritepadApp* exitsTo(void) { return BACK_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (BACK_APP is to LauncherApp, DEFAULT_APP is to a mouse capable app)
    virtual bool displaysClock(void) { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar

    virtual bool getEnabled(void) { return enabled; }
    virtual void setEnabled(bool e) { enabled = e; }

    AppMode getAppMode() { return currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);

    virtual bool canBeScreensaver(void) { return false; }
    virtual bool canBeInteractive(void) { return true; }
    virtual bool canBeMouse(void) { return false; }

    virtual bool isPopup(void) { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible(void) { return false; };    // has no UI

    virtual color_t bgColor(void) { return screen.black; }  // background color of app screen

    virtual bool displaysStatusBar(void) { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor(void) { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor(void) { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle(void) { return name(); }

    void drawStatusBar(bool update = false); // pass true to just do an update rather than a full redraw

    static BritepadApp* STAY_IN_APP;
    static BritepadApp* DEFAULT_APP; // typically the MouseApp, but might be a timer when it's running
    static BritepadApp* BACK_APP;  // return to launcher
    static BritepadApp* SCREENSAVER_APP; // go to a screensaver

  protected:
    bool enabled = true;
    coord_t statusBarTop = 0;
    coord_t statusBarHeight = 16;

    AppMode currAppMode = INACTIVE;

    virtual void clearScreen(void) { screen.fillScreen(bgColor()); }
};

#endif


