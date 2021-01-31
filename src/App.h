#ifndef _App_
#define _App_

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

class App {
  public:
    App() { addApp(this); };

    virtual const char* name() = 0;
    virtual appid_t id() = 0;

    inline App* getNextApp() { return _nextApp; }
    inline void setNextApp(App* app) {  _nextApp = app; };

    static App* getFirstApp() { return _appList; }
    static App* getAppByID(appid_t appID);
    static void sortApps();

    virtual void init() {};  // initialize app immediately after construction
    virtual void begin(AppMode asMode) {
      App::switchAppMode(asMode);
    };
    virtual void switchAppMode(AppMode asMode);  // called when switching between modes
    virtual void end(); // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs
    virtual void run() {};

    bool isID(appid_t match);

    AppMode getAppMode() { return _currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);

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

    virtual App* exitsTo() { return EXIT_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (EXIT_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)

    void launch() { launchApp(this); }
    void launchApp(App* app, AppMode mode = INTERACTIVE_MODE);
    void exit() { launchApp(exitsTo()); };

    virtual bool isCurrentApp();

    virtual bool disablesScreensavers() { return false; }

    static constexpr App* STAY_IN_APP = (App*)0;
    static constexpr App* SWITCH_TO_INTERACTIVE_MODE = (App*)1; // stay in app, but switch mode
    static constexpr App* A_MOUSE_APP = (App*)2; // typically the MouseApp, but might be a timer when it's running
    static constexpr App* A_SCREENSAVER_APP = (App*)3; // go to a screensaver
    static constexpr App* EXIT_APP = (App*)4;  // return to launcher
    static constexpr App* LAST_APP = (App*)5;  // return to the last app before this one.
    static constexpr bool validApp(App* a) { return a > LAST_APP; }

    virtual bool displaysClock() { return false; }  // todo - move to BritepadApp

  protected:
    virtual AppMode defaultEnabled() { return ANY_MODE; }
    AppMode _enabled;  // default is in defaultEnabled();
    AppMode _currAppMode = INACTIVE_MODE;
    static const millis_t _maxRunTime = 10;

    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse() | isAppType(KEYBOARD_APP); } // mice, screensavers and keyboard apps use default prefs for enable/disable

    virtual void setPrefs();
    virtual void getPrefs();

  private:
    App* _nextApp = nullptr;
    static App* _appList;

    static void setFirstApp(App* a) { _appList = a; }
    static void addApp(App* app) {
      app->setNextApp(_appList);
      _appList = app;
    }
};

#endif


