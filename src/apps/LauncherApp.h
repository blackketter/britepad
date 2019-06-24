#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"
#include "widgets/ButtonMatrix.h"

typedef int8_t pageindex_t;

class LauncherPage;

class LauncherApp : public BritepadApp {
  public:
    LauncherApp();
//    void init();
    void begin(AppMode asMode);
    void run();
    void end();
    bool event(KeyEvent* key);

    bool usesKeyboard() { return true; }
    BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }
    const char* name() { return "Launcher"; };

    AppType getAppType() { return NO_APP_TYPE; } // doesn't show a button for itself

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    const char* statusBarTitle();
    const char* infoBarText();

    bool disablesScreensavers();

  private:
    const static int noButton = -1;

    const static int h_buttons = 4;
    const static int v_buttons = 3;
    const static int buttons_per_page = h_buttons * v_buttons;
    const static int resetScreenTimeout = 10;  // seconds

    LauncherPage* getCurrentPage();
    pageindex_t getCurrentPageIndex() { return current_page; }
    void setCurrentPageIndex(pageindex_t n);
    pageindex_t firstPageOfType(AppType type);
    pageindex_t pageCount();

    void pushPage(direction_t d);
    void goToPage(pageindex_t s);
    color_t bgColor();

    void drawButtons();

    void makePages();
    void freePages();

    pageindex_t current_page;

    bool waitForRelease = false;

    time_t lastRun = 0;
    time_t lastBegin = 0;
    bool held = false;
    BritepadApp* launchOnRelease = nullptr;
    bool audibleExit = false;
    bool exitOnRelease = false;

    const AppType defaultPageAppType = KEY_APP;

    ButtonMatrix* buttons = 0;
    LauncherPage* _pages = nullptr;
};

#endif

