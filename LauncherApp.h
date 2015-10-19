#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"

enum ScreenNames {
  SETTINGS_SCREEN,
  SCREENSAVERS_SCREEN,
  KEYS_SCREEN,
  TIMERS_SCREEN,
  APPS_SCREEN,
  TOTAL_SCREENS
};

class LauncherApp : public BritepadApp {
  public:
    LauncherApp(void);
    void begin(AppMode asMode);
    void end(BritepadApp* newApp);
    BritepadApp* run(void);
    BritepadApp* exitsTo(void) { return DEFAULT_APP; }
    const char* name(void) { return "Launcher"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    const char* statusBarTitle(void);
    const char* infoBarText(void);

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    int buttonHit(int x, int y);
    void drawButton(int i, bool highlighted = false);
    void drawButtons(void);
    int currentScreen(void);
    color_t bgColor(void);
    void setButton(int screen, int i, BritepadApp* b);
    BritepadApp* getButton(int i);

    BritepadApp* apps[TOTAL_SCREENS][buttons_per_screen];

    const char* screenNames[TOTAL_SCREENS] = {
        "Settings",
        "Screensavers",
        "Keys",
        "Timers",
        "Apps"
      };

    const color_t screenColor[TOTAL_SCREENS] = {
        screen.darkerred,
        screen.darkeryellow,
        screen.darkergreen,
        screen.darkerblue,
        screen.darkergrey
    };

    int current_screen = 1;

    int highlighted_button = noButton;

    time_t lastRun = 0;
};
#endif

