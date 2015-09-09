#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "Debug.h"


// todo move this into the base object
#define BritepadAppScratchPadSize (32768L)
extern uint8_t BritepadAppScratchPad[];

class Britepad;

class BritepadApp {
  public:
    BritepadApp();
    virtual void begin(void) { screen.fillScreen(bgColor()); };  // initialize app state and draw first screen
    virtual void end(BritepadApp* nextApp) {}; // called after final run(), lets app clean up and tells it what the next app may be
    virtual BritepadApp* run(void) {return STAY_IN_APP;};  // run current app state repeatedly, returns pointer to next app to run (or one of the constants above)

    virtual const char* name(void) = 0;
    virtual color_t buttonColor(void) { return screen.blue; }

    virtual bool isScreensaver(void) { return false; };
    virtual bool disablesScreensavers(void) { return false; }
    virtual bool wantsToBeScreensaver(void) { return false; }  // return true if you want to be switched to as the screensaver
    virtual bool isPopup(void) { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible(void) { return false; };    // has no UI

    virtual color_t bgColor(void) { return screen.black; }  // background color of app screen

    virtual bool displaysStatusBar(void) { return !isScreensaver(); };  // apps show status bar by default
    virtual color_t statusBarBGColor(void) { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor(void) { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle(void) { return name(); }

    coord_t top(void);
    coord_t left(void);
    coord_t height(void);
    coord_t width(void);

    void updateStatusBarBounds();
    void drawStatusBar();

    static BritepadApp* STAY_IN_APP;
    static BritepadApp* DEFAULT_APP; // typically the MouseApp
    static BritepadApp* BACK_APP;  // return to launcher
    static BritepadApp* SCREENSAVER_APP; // go to a screensaver

  protected:
    coord_t statusBarTop = 0;
    coord_t statusBarHeight = 16;
};

#endif


