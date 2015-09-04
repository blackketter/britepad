#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"

class LauncherApp : public BritepadApp {
  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int total_screens = 3;
    const static int buttons_per_screen = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    BritepadApp* apps[total_screens][buttons_per_screen];

    int current_screen = 1;

    int highlighted_button = noButton;

    int buttonHit(int x, int y);
    void drawButton(int i, bool highlighted = false);
    void drawButtons(void);
    int currentScreen(void);
    color_t bgColor(void);
    time_t lastRun = 0;

  public:
    LauncherApp(void);
    void begin(void);
    void end(BritepadApp* newApp);
    BritepadApp* run(void);
    void setButton(int screen, int i, BritepadApp* b);
    BritepadApp* getButton(int i);
    const char* name(void) { return "Launcher"; };
};
#endif

