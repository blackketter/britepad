#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "widgets/ButtonMatrix.h"

typedef int (*appCompareFunction)(BritepadApp*, BritepadApp*);

class LauncherApp : public BritepadApp {
  public:
    void begin(AppMode asMode);
    void run();
    void end();
    void idle();

    BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }
    const char* name() { return "Launcher"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    bool usesKeyboard() { return true; }
    const char* statusBarTitle();
    const char* infoBarText();

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    BritepadApp* nextSortedApp(BritepadApp* last, appCompareFunction compareFunc) { return nullptr;};

    int getCurrentScreen() { return current_screen; }
    void setCurrentScreen(int n = KEYS_SCREEN);
    color_t bgColor();

    void clearScreen() {} // override the default clear screen because we do transitions
    void drawButtons();
    int current_screen = HOME_SCREEN;

    bool waitForRelease = false;

    time_t lastRun = 0;
    time_t lastBegin = 0;
    bool held = false;
    BritepadApp* launchOnRelease = nullptr;

    enum Screens {
      DEBUG_SCREEN,
      MICE_SCREEN,
      CLOCKS_SCREEN,
      SCREENSAVERS_SCREEN,
      SETTINGS_SCREEN,
      KEYS_SCREEN,
      HOME_SCREEN = KEYS_SCREEN,
      TIMERS_SCREEN,
      APPS_SCREEN,
      TOTAL_SCREENS
    };

    ButtonMatrix* buttons = 0;

    const char* infoText[TOTAL_SCREENS] = {
        nullptr,
        "Press and hold to test",
        "Press and hold to test",
        "Press and hold to test",
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    const char* screenNames[TOTAL_SCREENS] = {
        "Debug",
        "Mice",
        "Clocks",
        "Screensavers",
        "Settings",
        "Launcher",
        "Timers",
        "Apps",
      };

    const AppType screenTypes[TOTAL_SCREENS] = {
        DEBUG_APP,
        MOUSE_APP,
        CLOCK_APP,
        SCREENSAVER_APP,
        SETTINGS_APP,
        KEY_APP,
        TIMER_APP,
        INTERACTIVE_APP,
    };

    const AppMode screenMode[TOTAL_SCREENS] = {
        INTERACTIVE_MODE,
        MOUSE_MODE,
        SCREENSAVER_MODE,
        SCREENSAVER_MODE,
        INTERACTIVE_MODE,
        INTERACTIVE_MODE,
        INTERACTIVE_MODE,
        INTERACTIVE_MODE,
    };

    const color_t screenColor[TOTAL_SCREENS] = {
        screen.lightgrey,
        screen.black,
        screen.darkerred,
        screen.darkeryellow,
        screen.darkeryellow,
        screen.darkergreen,
        screen.darkerblue,
        screen.darkergrey,
    };
};

#endif

