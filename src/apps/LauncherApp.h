#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "widgets/Button.h"
#include "widgets/ButtonMatrix.h"

class AppButton : public RoundButton {
  public:
    BritepadApp* getApp() { return (BritepadApp*)getID(); }
    void setApp(BritepadApp* a)  {  setID((widgetid_t)a); }
};


class AppButtonMatrix : public ButtonMatrix {
  public:
    AppButtonMatrix(coord_t x, coord_t y, coord_t w, coord_t h, int rows, int columns, int maps, ButtonConfig configuration[] = nullptr) {
      init(x, y, w, h, rows, columns, maps, configuration);
    };

  protected:
    Button* newButton() { return new AppButton(); }
};

class LauncherApp : public BritepadApp {
  public:
    LauncherApp();
    void begin(AppMode asMode);
    void end();
    void run();
    BritepadApp* exitsTo() { return MOUSE_APP; }
    const char* name() { return "Launcher"; };

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

    int buttonHit(int x, int y);
    void drawButton(int i, bool highlighted = false);
    void drawButtons();
    int getCurrentScreen() { return current_screen; }
    void setCurrentScreen(int n) { current_screen = n; }
    color_t bgColor();
    AppMode screenMode(int theScreen);
    void setButton(int screen, int i, BritepadApp* b);
    BritepadApp* getButton(int i);
    void checkTimeout();
    void clearScreen() {} // override the default clear screen because we do transitions

    enum ScreenNames {
      DEBUG_SCREEN,
      MICE_SCREEN,
      CLOCKS_SCREEN,
      SCREENSAVERS_SCREEN,
      SETTINGS_SCREEN,
      KEYS_SCREEN,
      TIMERS_SCREEN,
      APPS_SCREEN,
      TOTAL_SCREENS
    };

    BritepadApp* apps[TOTAL_SCREENS][buttons_per_screen];

    AppButtonMatrix* buttons;

    const char* screenNames[TOTAL_SCREENS] = {
        "Debug",
        "Mice",
        "Clocks",
        "Screensavers",
        "Settings",
        "Keys",
        "Timers",
        "Apps",
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

    int current_screen = SETTINGS_SCREEN;

    int highlighted_button = noButton;

    time_t lastRun = 0;
    static const millis_t holdTime = 500;
    BritepadApp* launchOnRelease = nullptr;
};

#endif

