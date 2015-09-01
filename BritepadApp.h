#ifndef _BritepadApp_
#define _BritepadApp_

#include "Britepad.h"
#include "Debug.h"

#define STAY_IN_APP ((BritepadApp*)0)
#define DEFAULT_APP ((BritepadApp*)1)
#define BACK_APP ((BritepadApp*)2)

// todo move this into the base object
#define BritepadApp_SCRATCH_PAD_SIZE ((long)32768)
extern uint8_t BritepadApp_SCRATCH_PAD[];

class BritepadApp {
  public:
    BritepadApp() {};
    virtual void begin(void) { screen.fillScreen(bgColor()); };  // initialize app state and draw first screen
    virtual void end(BritepadApp* nextApp) {}; // called after final run(), lets app clean up and tells it what the next app may be
    virtual BritepadApp* run(void) {return nil;};  // run current app state repeatedly, returns pointer to next app to run (or one of the constants above)
    virtual const char* name(void) { return 0; };
    virtual color_t buttonColor(void) { return screen.blue; }

    virtual bool isScreensaver(void) { return false; };
    virtual bool disablesScreensavers(void) { return false; }
    virtual bool isPopup(void) { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible(void) { return false; };    // has no UI

    virtual color_t bgColor(void) { return screen.black; }  // background color of app screen

    virtual bool displaysStatusBar(void) { return !isScreensaver(); };  // apps show status bar by default
    virtual color_t statusBarBGColor(void) { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor(void) { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar

    coord_t top(void);
    coord_t left(void);
    coord_t height(void);
    coord_t width(void);

    void updateStatusBar(bool redraw); // call redraw if you need to invalidate and redraw the entire bar.  May cause flickering.

  protected:
    coord_t statusBarTop = 0;
    coord_t statusBarHeight = 16;
};

#endif


