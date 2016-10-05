#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "Debug.h"
#include "widgets/Icon.h"

class Britepad;


// Apps are of particular types, which may (for example) have them automatically placed on a screen
enum AppType {
  NO_APP_TYPE = 0,
  DEBUG_APP = 1,
  SETTINGS_APP = 2,
  MOUSE_APP = 4,
  CLOCK_APP = 8,
  SCREENSAVER_APP = 16,
  INTERACTIVE_APP = 32,
  TIMER_APP = 64,
  KEY_APP = 128,
};

class BritepadApp {
  public:
    BritepadApp();
    virtual void init() {};  // initialize app immediately after boot
    virtual void begin(AppMode asMode);  // initialize app state and draw first screen, allocate any memory needed while running
    virtual void end(); // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs

    virtual void run() { if (isAppMode(MOUSE_MODE)) { mouse.run(); } };  // run current app state repeatedly, returns pointer to next app to run (or one of the constants below)
    virtual void switchAppMode(AppMode asMode);  // called when switching between modes

    static BritepadApp* STAY_IN_APP;
    static BritepadApp* A_MOUSE_APP; // typically the MouseApp, but might be a timer when it's running
    static BritepadApp* BACK_APP;  // return to launcher
    static BritepadApp* A_SCREENSAVER_APP; // go to a screensaver

    virtual const char* name() = 0;
    virtual Icon getIcon() { return icon; };
    virtual appid_t id() = 0;
    bool isID(appid_t match);
    BritepadApp* getAppByID(appid_t getID) { return britepad.getAppByID(getID); }

    virtual color_t buttonColor() { return screen.blue; }

    virtual bool isCurrentApp() { return britepad.currentApp() == this; };

    virtual bool disablesScreensavers() { return false; }
    virtual bool wantsToBe(AppMode m) { return false; }  // return true if you want to be switched to as the specified mode (canBeMODENAME() doesn't have to be true)
    virtual BritepadApp* exitsTo() { return BACK_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (BACK_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)
    virtual bool displaysClock() { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar
    virtual bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual bool getEnabled(AppMode asMode = ANY_MODE) { readPrefs(); return (bool)(enabled & asMode); }
    virtual void setEnabled(bool e, AppMode asMode = ANY_MODE) {
      if (e) {
        enabled = (AppMode)(enabled | asMode);
      } else {
        enabled = (AppMode)(enabled & (~asMode));
      }
      writePrefs();
    }

    AppMode getAppMode() { return currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);
    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE) { britepad.launchApp(app, mode); };
    void exit() { launchApp(A_MOUSE_APP); };

    virtual bool canBeScreensaver() { return false; }
    virtual bool canBeInteractive() { return true; }
    virtual bool canBeMouse() { return false; }

    virtual AppType getAppType() { return NO_APP_TYPE; }
    virtual bool isAppType(AppType t) { return t & getAppType(); }
    virtual int32_t getLauncherPosition() { return position; }
    virtual void setLauncherPosition(int32_t p) { position = p; }
    virtual bool isHidden() { return false; }

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

    BritepadApp* getNextApp() {  return nextApp; };
    BritepadApp* getPrevApp() {  return prevApp; };
    void setNextApp(BritepadApp* app) {  nextApp = app; };
    void setPrevApp(BritepadApp* app) {  prevApp = app; };

  protected:
    AppMode enabled = ANY_MODE;  // apps are always enabled by default
    coord_t statusBarHeight = 16;
    Icon icon;
    static const int32_t defaultLauncherPosition = -1;
    int32_t position = defaultLauncherPosition;

    AppMode currAppMode = INACTIVE_MODE;

    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse(); } // mice and screensavers have prefs

    virtual void clearScreen() { screen.fillScreen(bgColor()); }
    void resetClipRect();  // resets clip rect to content area

    virtual void writePrefs() {  if (hasPrefs()) { uint8_t pref = (uint8_t)enabled; prefs.write(id(), sizeof(pref), (uint8_t*)&pref); } };
    virtual void readPrefs() { if (hasPrefs()) { uint8_t pref = (uint8_t)ANY_MODE; prefs.read(id(),  sizeof(pref), (uint8_t*)&pref); enabled = (AppMode)pref;} };

  private:
    BritepadApp* nextApp = nullptr;
    BritepadApp* prevApp = nullptr;
};

#endif


