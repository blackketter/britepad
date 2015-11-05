#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "Debug.h"
#include "Icon.h"

class Britepad;

class BritepadApp {
  public:
    BritepadApp();
    virtual void begin();  // initialize app state and draw first screen
    virtual void end(BritepadApp* nextApp); // called after final run(), lets app clean up and tells it what the next app may be

    virtual void run() { if (isAppMode(MOUSE)) { mouse.run(); } };  // run current app state repeatedly, returns pointer to next app to run (or one of the constants below)
    virtual void setAppMode(AppMode asMode);  // called automatically by begin() or when switching between modes

    static BritepadApp* STAY_IN_APP;
    static BritepadApp* DEFAULT_APP; // typically the MouseApp, but might be a timer when it's running
    static BritepadApp* BACK_APP;  // return to launcher
    static BritepadApp* SCREENSAVER_APP; // go to a screensaver

    virtual const char* name() = 0;
    virtual Icon getIcon() { return icon; };
    virtual appid_t id() = 0;
    bool isID(appid_t match);
    BritepadApp* getApp(appid_t getID) { return britepad.getApp(getID); }

    virtual color_t buttonColor() { return screen.blue; }

    virtual bool isCurrentApp() { return britepad.currentApp() == this; };

    virtual bool disablesScreensavers() { return false; }
    virtual bool wantsToBeScreensaver() { return false; }  // return true if you want to be switched to as the screensaver (canBeScreensaver() doesn't have to be true)
    virtual BritepadApp* exitsTo() { return BACK_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (BACK_APP is to LauncherApp, DEFAULT_APP is to a mouse capable app)
    virtual bool displaysClock() { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar

    virtual bool getEnabled() { return enabled; }
    virtual void setEnabled(bool e) { enabled = e; }

    AppMode getAppMode() { return currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);
    void setNextApp(BritepadApp* app, AppMode mode = INTERACTIVE) { britepad.setNextApp(app, mode); };
    void exit() { setNextApp(DEFAULT_APP); };

    virtual bool canBeScreensaver() { return false; }
    virtual bool canBeInteractive() { return true; }
    virtual bool canBeMouse() { return false; }

    virtual bool isPopup() { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible() { return false; };    // has no UI

    virtual color_t bgColor() { return screen.black; }  // background color of app screen

    void drawBars(bool update = false); // draw the status and info bars

    void drawStatusBar(bool update);
    virtual bool displaysStatusBar() { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor() { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor() { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle() { return name(); }

    void drawInfoBar(bool update);
    virtual bool displaysInfoBar() { return infoBarText() != nullptr; };
    virtual color_t infoBarBGColor() { return bgColor(); }
    virtual color_t infoBarFGColor() { return screen.luminance(infoBarBGColor()) > 127 ? screen.grey : screen.lightgrey; }  // info text is faded, by default
    virtual const char* infoBarText() { return nullptr; }


  protected:
    bool enabled = true;
    coord_t statusBarHeight = 16;
    Icon icon;

    AppMode currAppMode = INACTIVE;

    virtual void clearScreen() { screen.fillScreen(bgColor()); }
    void resetClipRect();  // resets clip rect to content area
    virtual void releaseMem() {};  // release any memory temporarily allocated by the app.  called automatically by BritepadApp::end()


};

#endif


