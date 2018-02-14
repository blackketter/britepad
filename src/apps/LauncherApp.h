#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "widgets/ButtonMatrix.h"

typedef uint8_t screenid_t;

enum screenids {
  FIRST_SCREEN,
  DEBUG_SCREEN = FIRST_SCREEN,
  MICE_SCREEN,
  CLOCKS_SCREEN,
  SCREENSAVERS_SCREEN,
  KEYBOARD_SCREEN,
  SETTINGS_SCREEN,
  MACROS_SCREEN,
  HOME_SCREEN,
  TIMERS_SCREEN,
  APPS_SCREEN,
  LAST_SCREEN = APPS_SCREEN,
  TOTAL_SCREENS,
  NO_SCREEN
};

typedef struct screen_t {
  screenid_t id;
  const char* name;
  const char* info;
  AppType type;
  AppMode mode;
  color_t color;
} screen_t;

class LauncherApp : public BritepadApp {
  public:
    LauncherApp();
    void begin(AppMode asMode);
    void run();
    void end();
    EventPriority eventPriority() { return PRIORITY_LAST; }
    bool event(KeyEvent* key);

    bool usesKeyboard() { return true; }
    BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }
    const char* name() { return "Launcher"; };

    AppType getAppType() { return NO_APP_TYPE; } // doesn't show a button for itself

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    const char* statusBarTitle();
    const char* infoBarText();

    void setLaunchScreen(screenid_t s) {launch_screen = s;}

    bool disablesScreensavers();

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    screenid_t getCurrentScreenID() { return current_screen; }
    void setCurrentScreenID(screenid_t n);
    void pushScreen(direction_t d);
    void goToScreen(screenid_t s);
    screen_t* getCurrentScreen();
    color_t bgColor();

    void drawButtons();
    screenid_t current_screen;
    screenid_t launch_screen = NO_SCREEN;

    bool waitForRelease = false;

    time_t lastRun = 0;
    time_t lastBegin = 0;
    bool held = false;
    BritepadApp* launchOnRelease = nullptr;
    bool audibleExit = false;
    bool exitOnRelease = false;

    ButtonMatrix* buttons = 0;
};

#endif

