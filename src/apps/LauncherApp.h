#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "widgets/ButtonMatrix.h"

typedef uint8_t screenid_t;

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
    void idle();

    bool usesKeyboard() { return true; }
    BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }
    const char* name() { return "Launcher"; };

    AppType getAppType() { return KEYBOARD_APP; }  // appears on keyboard screen because it has a hotkey

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    const char* statusBarTitle();
    const char* infoBarText();

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    screenid_t getCurrentScreenID() { return current_screen; }
    void setCurrentScreenID(screenid_t n);
    void pushScreen(direction_t d);
    screen_t* getCurrentScreen();
    color_t bgColor();

    void clearScreen() {} // override the default clear screen because we do transitions
    void drawButtons();
    screenid_t current_screen;

    bool waitForRelease = false;

    time_t lastRun = 0;
    time_t lastBegin = 0;
    bool held = false;
    BritepadApp* launchOnRelease = nullptr;

    ButtonMatrix* buttons = 0;
};

#endif

