#ifndef _BritepadApp_
#define _BritepadApp_

#include "BritepadShared.h"
#include "widgets/Icon.h"

class Britepad;
class AppButton;

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
  KEYBOARD_APP = 256,
};

class BritepadApp {
  public:
    BritepadApp();
    virtual void init() {};  // initialize app immediately after boot
    virtual void begin(AppMode asMode);  // initialize app state and draw first screen, allocate any memory needed while running
    virtual void end(); // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs

    virtual void run() { if (isAppMode(MOUSE_MODE)) { mouse.run(); } };  // run current app state repeatedly
    virtual void switchAppMode(AppMode asMode);  // called when switching between modes

    virtual void idle(KeyEvent* key) {};  // give apps an opportuntity to run in the background, useful for processing keyboard events

    static BritepadApp* STAY_IN_APP;
    static BritepadApp* A_MOUSE_APP; // typically the MouseApp, but might be a timer when it's running
    static BritepadApp* BACK_APP;  // return to launcher
    static BritepadApp* A_SCREENSAVER_APP; // go to a screensaver

    virtual const char* name() = 0;
    virtual Icon getIcon() { return nullptr; };
    virtual appid_t id() = 0;
    bool isID(appid_t match);
    BritepadApp* getAppByID(appid_t getID) { return britepad.getAppByID(getID); }

    virtual bool highlighted() { return wantsToRun(); }  // by default, apps are highlighted if they want to be displayed

    virtual bool isCurrentApp() { return britepad.currentApp() == this; };

    virtual bool disablesScreensavers() { return false; }
    virtual bool wantsToRun() { return false; }  // return true if you want to be switched to (canBeMODENAME() doesn't have to be true)
    virtual BritepadApp* exitsTo() { return BACK_APP; }  // when exiting the app, typically by the TOP_PAD, where should it go by default (BACK_APP is to LauncherApp, MOUSE_APP is to a mouse capable app)
    virtual bool displaysClock() { return false; }  // return true if the content includes a clock, otherwise we'll put a clock in the status bar
    virtual bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual bool getEnabled(AppMode asMode = ANY_MODE) { readPrefs(); return (bool)(_enabled & asMode); }
    virtual void setEnabled(bool e, AppMode asMode = ANY_MODE) {
      if (e) {
        _enabled = (AppMode)(_enabled | asMode);
      } else {
        _enabled = (AppMode)(_enabled & (~asMode));
      }
      writePrefs();
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
    virtual bool displaysStatusBar() { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor() { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor() { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle() { return name(); }

    void drawInfoBar(bool update);
    virtual bool displaysInfoBar() { return infoBarText() != nullptr; };
    virtual color_t infoBarBGColor() { return bgColor(); }
    virtual color_t infoBarFGColor() { return screen.luminance(infoBarBGColor()) > 127 ? screen.grey : screen.lightgrey; }  // info text is faded, by default
    virtual const char* infoBarText() { return nullptr; }

    inline BritepadApp* getNextApp() {  return _nextApp; };
    inline BritepadApp* getPrevApp() {  return _prevApp; };
    inline void setNextApp(BritepadApp* app) {  _nextApp = app; };
    inline void setPrevApp(BritepadApp* app) {  _prevApp = app; };


  protected:
    KeyEvent* getNextEvent() { return keys.getNextEvent(); }
    virtual bool hasPrefs() { return canBeScreensaver() | canBeMouse() | isAppType(KEYBOARD_APP); } // mice, screensavers and keyboard apps use default prefs for enable/disable

    virtual void clearScreen();
    void resetClipRect();  // resets clip rect to content area

    virtual void writePrefs() {  if (hasPrefs()) { uint8_t pref = (uint8_t)_enabled; prefs.write(id(), sizeof(pref), (uint8_t*)&pref); } };
    virtual void readPrefs() { if (hasPrefs()) { uint8_t pref = (uint8_t)ANY_MODE; prefs.read(id(),  sizeof(pref), (uint8_t*)&pref); _enabled = (AppMode)pref;} };

    static const coord_t _statusBarHeight = 16;
    static const int32_t _defaultLauncherPosition = -1;
    static const millis_t _maxRunTime = 10;

    AppMode _enabled = ANY_MODE;  // bit mask for enabled modes. apps are always enabled by default
    AppMode _currAppMode = INACTIVE_MODE;

    int32_t _launcherPosition = _defaultLauncherPosition;

  private:

    BritepadApp* _nextApp = nullptr;
    BritepadApp* _prevApp = nullptr;
};

#endif


