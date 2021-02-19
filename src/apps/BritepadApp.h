#ifndef _BritepadApp_
#define _BritepadApp_

#include "App.h"
#include "widgets/Icon.h"

// Apps run in particular modes
enum AppMode {
  NO_MODE = 0,
  INACTIVE_MODE = 0, // set to this mode to exit
  MOUSE_MODE = 1,
  INTERACTIVE_MODE = 2,
  SCREENSAVER_MODE = 4,
  // 8, // available, was INACTIVE_MODE
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

class BritepadApp : public App {
  public:

    void begin() override;  // initialize app state and draw first screen, allocate any memory needed while running
    void end() override;

    virtual void setAppMode(AppMode asMode);  // called when switching between modes

    AppMode getAppMode() { return _currAppMode; }
    bool isAppMode(AppMode is) { return (is == getAppMode()); }
    bool canBeAppMode(AppMode b);

    bool isCurrentApp();
    void launch();

    static BritepadApp* getFirstBritepadApp() { return (BritepadApp*)getFirstApp(); }
    BritepadApp* getNextBritepadApp() { return (BritepadApp*)(getNextApp()); }

    KeyEvent* getNextEvent();
    KeyEvent* peekNextEvent();

    bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual bool canBeScreensaver() { return false; }
    virtual bool canBeInteractive() { return true; }
    virtual bool canBeMouse() { return false; }
    virtual bool canBeSetup() { return false; }
    virtual bool canBeInvisible() { return false; }     // has no UI
    virtual bool usesKeyboard() { return false; }

    void exit() { setAppMode(INACTIVE_MODE); }

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

    virtual bool disablesScreensavers() { return false; }

    static constexpr BritepadApp* EXIT_APP = (BritepadApp*)1;  // return to launcher
    static constexpr BritepadApp* LAST_APP = (BritepadApp*)2;  // return to the last app before this one.

    static constexpr BritepadApp* SWITCH_TO_INTERACTIVE_MODE = (BritepadApp*)3; // stay in app, but switch mode
    static constexpr BritepadApp* A_MOUSE_APP = (BritepadApp*)4; // typically the MouseApp, but might be a timer when it's running
    static constexpr BritepadApp* A_SCREENSAVER_APP = (BritepadApp*)5; // go to a screensaver

    static constexpr bool validApp(App* a) { return a > A_SCREENSAVER_APP; }

    virtual BritepadApp* exitsTo() { return EXIT_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (EXIT_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)
    virtual bool wantsToRun() { return false; }  // return true if you want to be switched to (canBeMODENAME() doesn't have to be true)

    virtual bool displaysClock() { return false; }  // todo - move to BritepadApp

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
    void setClipRect(coord_t x, coord_t y, coord_t w, coord_t h);  // resets clip rect to content area

    virtual bool highlighted() { return wantsToRun(); }  // by default, apps are highlighted if they want to be displayed
    virtual bool isScreenApp() { return true; }

// TODO - move these to base class
    void delay(millis_t d);  // an idle aware delay
    bool timeToLeave();

    virtual AppType getAppType() { return NO_APP_TYPE; }
    virtual bool isAppType(AppType t) { return t & getAppType(); }

    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse() | isAppType(KEYBOARD_APP); } // mice, screensavers and keyboard apps use default prefs for enable/disable

    virtual void setPrefs();
    virtual void getPrefs();

  protected:

    virtual AppMode defaultEnabled() { return ANY_MODE; }

    AppMode _enabled;  // default is in defaultEnabled();
    AppMode _currAppMode = INACTIVE_MODE;

    int32_t _launcherPosition = defaultLauncherPosition;

};

#endif


