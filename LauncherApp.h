#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"

enum ScreenNames {
  SETTINGS_SCREEN,
  KEYS_SCREEN,
  TIMERS_SCREEN,
  APPS_SCREEN
};

class LauncherApp : public BritepadApp {
  public:
    LauncherApp(void);
    void begin(void);
    void end(BritepadApp* newApp);
    BritepadApp* run(void);
    void setButton(int screen, int i, BritepadApp* b);
    BritepadApp* getButton(int i);
    const char* name(void) { return "Launcher"; };
    const char* statusBarTitle(void);

    appid_t id() { return ID; }
    static constexpr appid_t ID = "laun";

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int total_screens = 4;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    BritepadApp* apps[total_screens][buttons_per_screen];

    const char* screenNames[total_screens] = {
        "Settings",
        "Keys",
        "Timers",
        "Apps"
      };

    const color_t screenColor[total_screens] = {
        screen.darkerred,
        screen.darkergreen,
        screen.darkerblue,
        screen.darkergrey
    };

    int current_screen = 1;

    int highlighted_button = noButton;

    int buttonHit(int x, int y);
    void drawButton(int i, bool highlighted = false);
    void drawButtons(void);
    int currentScreen(void);
    color_t bgColor(void);
    time_t lastRun = 0;
};
#endif

