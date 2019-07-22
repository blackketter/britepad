#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "widgets/Icon.h"
#include "widgets/Widget.h"

class Britepad;
class AppButton;

// Apps are of particular types, which may (for example) have them automatically placed on the launcher
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
  KEYBOARD_APP = 256,
  MACROS_APP = 512,
  ANY_TYPE_APP = 0xffff,
};

enum EventPriority {
    PRIORITY_FIRST = -1000,
    PRIORITY_EARLY = -500,
    PRIORITY_MIDDLE = 0,
    PRIORITY_NORMAL = 500,
    PRIORITY_LAST = 1000,
    PRIORITY_END = 9999,
};

class BritepadApp : public Widget {
  public:
    BritepadApp();
    virtual void init() {};  // initialize app immediately after boot
    virtual void begin(AppMode asMode);  // initialize app state and draw first screen, allocate any memory needed while running
    virtual void end(); // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs

    virtual void run() {};
    virtual void switchAppMode(AppMode asMode);  // called when switching between modes

    // give apps an opportuntity to run in the background, useful for processing keyboard events before they go to the currently running app or off to host
    // return if the event is consumed and should not be further processed
    virtual bool event(KeyEvent* key);
    virtual EventPriority eventPriority() { return PRIORITY_NORMAL; };

    virtual void timeChanged() {};  // notify app that wall clock time has been adjusted

    static constexpr BritepadApp* STAY_IN_APP = (BritepadApp*)0;
    static constexpr BritepadApp* SWITCH_TO_INTERACTIVE_MODE = (BritepadApp*)1; // stay in app, but switch mode
    static constexpr BritepadApp* A_MOUSE_APP = (BritepadApp*)2; // typically the MouseApp, but might be a timer when it's running
    static constexpr BritepadApp* A_SCREENSAVER_APP = (BritepadApp*)3; // go to a screensaver
    static constexpr BritepadApp* EXIT_APP = (BritepadApp*)4;  // return to launcher
    static constexpr BritepadApp* LAST_APP = (BritepadApp*)5;  // return to the last app before this one.
    static constexpr bool validApp(BritepadApp* a) { return a > LAST_APP; }

    virtual const char* name() = 0;
    virtual Icon getIcon() { return nullptr; };
    virtual appid_t id() = 0;
    bool isID(appid_t match);
    BritepadApp* getAppByID(appid_t getID) { return britepad.getAppByID(getID); }

    virtual bool highlighted() { return wantsToRun(); }  // by default, apps are highlighted if they want to be displayed

    virtual bool isCurrentApp() { return britepad.currentApp() == this; };

    virtual bool disablesScreensavers() { return false; }
    virtual bool wantsToRun() { return false; }  // return true if you want to be switched to (canBeMODENAME() doesn't have to be true)
    virtual BritepadApp* exitsTo() { return EXIT_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (EXIT_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)
    virtual bool displaysClock() { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar
    virtual bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual bool getEnabled(AppMode asMode = ANY_MODE) { getPrefs(); return (bool)(_enabled & asMode); }
    virtual void setEnabled(bool e, AppMode asMode = ANY_MODE) {
      if (e) {
        _enabled = (AppMode)(_enabled | asMode);
      } else {
        _enabled = (AppMode)(_enabled & (~asMode));
      }
      setPrefs();
    }

    AppMode getAppMode() { return _currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);
    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE) { britepad.launchApp(app, mode); };
    void exit() { launchApp(exitsTo()); };
    void launch() { launchApp(this); }
    bool timeToLeave() { return ((Uptime::millis() - pad.time()) > _maxRunTime); }
    void delay(millis_t d);  // an idle aware delay

    virtual bool canBeScreensaver() { return false; }
    virtual bool canBeInteractive() { return true; }
    virtual bool canBeMouse() { return false; }
    virtual bool canBeSetup() { return false; }
    virtual bool canBeInvisible() { return false; }     // has no UI
    virtual bool usesKeyboard() { return false; }

    virtual AppType getAppType() { return NO_APP_TYPE; }
    virtual bool isAppType(AppType t) { return t & getAppType(); }
    virtual int32_t getLauncherPosition() { return _launcherPosition; }
    virtual void setLauncherPosition(int32_t p) { _launcherPosition = p; }
    virtual AppButton* newAppButton();
    virtual color_t appButtonColor() { return screen.CadetBlue; }

    virtual bool isHidden() { return false; }

    virtual bool isInvisible() { return false; };

    virtual color_t bgColor() { return screen.black; }  // background color of app screen

    void drawBars(bool update = false); // draw the status and info bars

    void drawStatusBar(bool update);
    virtual void clearScreen();

    virtual bool displaysStatusBar() { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor() { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor() { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle() { return name(); }

    void drawInfoBar(bool update = false);
    virtual bool displaysInfoBar() { return infoBarText() != nullptr; };
    virtual color_t infoBarBGColor() { return bgColor(); }
    virtual color_t infoBarFGColor() { return screen.luminance(infoBarBGColor()) > 127 ? screen.grey : screen.lightgrey; }  // info text is faded, by default
    virtual const char* infoBarText() { return nullptr; }

    inline BritepadApp* getNextApp() {  return _nextApp; };
    inline BritepadApp* getPrevApp() {  return _prevApp; };
    inline void setNextApp(BritepadApp* app) {  _nextApp = app; };
    inline void setPrevApp(BritepadApp* app) {  _prevApp = app; };
    static const int32_t defaultLauncherPosition = -1;


  protected:
    KeyEvent* getNextEvent() { KeyEvent* e = keyEvents.getNextEvent(); if (e) { britepad.resetScreensaver(); }; return e;}
    KeyEvent* peekNextEvent() { KeyEvent* e = keyEvents.peekNextEvent(); if (e) { britepad.resetScreensaver(); }; return e;}
    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse() | isAppType(KEYBOARD_APP); } // mice, screensavers and keyboard apps use default prefs for enable/disable

    void resetClipRect();  // resets clip rect to content area

    virtual void setPrefs() {  if (hasPrefs()) { uint8_t pref = (uint8_t)_enabled; prefs.set(id(), sizeof(pref), (uint8_t*)&pref); } };
    virtual void getPrefs() { if (hasPrefs()) { uint8_t pref = (uint8_t)ANY_MODE; prefs.get(id(),  sizeof(pref), (uint8_t*)&pref); _enabled = (AppMode)pref;} };

    static const coord_t _statusBarHeight = 16;
    static const millis_t _maxRunTime = 10;

    AppMode _enabled = ANY_MODE;  // bit mask for enabled modes. apps are always enabled by default
    AppMode _currAppMode = INACTIVE_MODE;

    int32_t _launcherPosition = defaultLauncherPosition;

  private:

    BritepadApp* _nextApp = nullptr;
    BritepadApp* _prevApp = nullptr;
};

#endif


