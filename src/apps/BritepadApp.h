#ifndef _BritepadApp_
#define _BritepadApp_

#include "widgets/Icon.h"
#include "KeyEvent.h"

enum EventPriority {
    PRIORITY_FIRST = -1000,
    PRIORITY_EARLY = -500,
    PRIORITY_MIDDLE = 0,
    PRIORITY_NORMAL = 500,
    PRIORITY_LAST = 1000,
    PRIORITY_END = 9999,
};

typedef const char* appid_t;

// Apps run in particular modes
enum AppMode {
  NO_MODE = 0,
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  INACTIVE_MODE = 8,
  KEYBOARD_MODE = 16,
  INVISIBLE_MODE = 32,
  SETUP_MODE = 64,
  ANY_MODE = 255
};

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

class AppButton;

class BritepadApp {
  public:
    BritepadApp() { addApp(this); };

    virtual const char* name() = 0;
    virtual appid_t id() = 0;

    inline BritepadApp* getNextApp() { return _nextApp; }
    inline void setNextApp(BritepadApp* app) {  _nextApp = app; };

    static BritepadApp* getFirstApp() { return _appList; }
    static BritepadApp* getAppByID(appid_t appID);
    static void sortApps();

    virtual void init() {};  // initialize app immediately after boot
    virtual void begin(AppMode asMode);  // initialize app state and draw first screen, allocate any memory needed while running
    virtual void switchAppMode(AppMode asMode);  // called when switching between modes
    virtual void end(); // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs
    virtual void run() {};

    bool isID(appid_t match);


    virtual bool wantsToRun() { return false; }  // return true if you want to be switched to (canBeMODENAME() doesn't have to be true)

    virtual bool getEnabled(AppMode asMode = ANY_MODE) {
      getPrefs();
      return (bool)(_enabled & asMode);
    }

    virtual void setEnabled(bool e, AppMode asMode = ANY_MODE) {
      if (e) {
        _enabled = (AppMode)(_enabled | asMode);
      } else {
        _enabled = (AppMode)(_enabled & (~asMode));
      }
      setPrefs();
    }

    // give apps an opportuntity to run in the background, useful for processing keyboard events before they go to the currently running app or off to host
    // return if the event is consumed and should not be further processed
    virtual bool event(KeyEvent* key);
    virtual EventPriority eventPriority() { return PRIORITY_NORMAL; };

    virtual bool canBeScreensaver() { return false; }
    virtual bool canBeInteractive() { return true; }
    virtual bool canBeMouse() { return false; }
    virtual bool canBeSetup() { return false; }
    virtual bool canBeInvisible() { return false; }     // has no UI
    virtual bool usesKeyboard() { return false; }

    virtual AppType getAppType() { return NO_APP_TYPE; }
    virtual bool isAppType(AppType t) { return t & getAppType(); }

    virtual bool displaysClock() { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar
    virtual bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual int32_t getLauncherPosition() { return _launcherPosition; }
    virtual void setLauncherPosition(int32_t p) { _launcherPosition = p; }
    virtual AppButton* newAppButton();
    virtual color_t appButtonColor() { return screen.CadetBlue; }

    virtual bool isHidden() { return false; }

    virtual bool isInvisible() { return false; };

    virtual color_t bgColor() { return screen.black; }  // background color of app screen

    virtual void clearScreen();

    virtual bool displaysStatusBar() { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor() { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor() { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle() { return name(); }

    virtual bool displaysInfoBar() { return infoBarText() != nullptr; };
    virtual color_t infoBarBGColor() { return bgColor(); }
    virtual color_t infoBarFGColor() { return screen.luminance(infoBarBGColor()) > 127 ? screen.grey : screen.lightgrey; }  // info text is faded, by default
    virtual const char* infoBarText() { return nullptr; }

    static const int32_t defaultLauncherPosition = -1;

    virtual Icon getIcon() { return nullptr; };

    AppMode getAppMode() { return _currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);
     void setClipRect(coord_t x, coord_t y, coord_t w, coord_t h);  // resets clip rect to content area

    virtual bool highlighted() { return wantsToRun(); }  // by default, apps are highlighted if they want to be displayed
    virtual bool isScreenApp() { return true; }
    virtual BritepadApp* exitsTo() { return EXIT_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (EXIT_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)

    void launch() { launchApp(this); }
    void launchApp(BritepadApp* app, AppMode mode = INTERACTIVE_MODE);
    void exit() { launchApp(exitsTo()); };

    virtual bool isCurrentApp();
    bool timeToLeave();
    void delay(millis_t d);  // an idle aware delay

    virtual bool disablesScreensavers() { return false; }

    static constexpr BritepadApp* STAY_IN_APP = (BritepadApp*)0;
    static constexpr BritepadApp* SWITCH_TO_INTERACTIVE_MODE = (BritepadApp*)1; // stay in app, but switch mode
    static constexpr BritepadApp* A_MOUSE_APP = (BritepadApp*)2; // typically the MouseApp, but might be a timer when it's running
    static constexpr BritepadApp* A_SCREENSAVER_APP = (BritepadApp*)3; // go to a screensaver
    static constexpr BritepadApp* EXIT_APP = (BritepadApp*)4;  // return to launcher
    static constexpr BritepadApp* LAST_APP = (BritepadApp*)5;  // return to the last app before this one.
    static constexpr bool validApp(BritepadApp* a) { return a > LAST_APP; }

  protected:
    virtual AppMode defaultEnabled() { return ANY_MODE; }
    AppMode _enabled;  // default is in defaultEnabled();
    AppMode _currAppMode = INACTIVE_MODE;
    static const millis_t _maxRunTime = 10;

    KeyEvent* getNextEvent();
    KeyEvent* peekNextEvent();
    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse() | isAppType(KEYBOARD_APP); } // mice, screensavers and keyboard apps use default prefs for enable/disable

    virtual void setPrefs();
    virtual void getPrefs();

    int32_t _launcherPosition = defaultLauncherPosition;

  private:
    BritepadApp* _nextApp = nullptr;
    static BritepadApp* _appList;

    static void setFirstApp(BritepadApp* a) { _appList = a; }
    static void addApp(BritepadApp* app) {
      app->setNextApp(_appList);
      _appList = app;
    }

};

#endif


