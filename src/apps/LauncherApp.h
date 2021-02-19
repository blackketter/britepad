#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BritepadShared.h"
#include "BritepadApp.h"

typedef int8_t pageindex_t;

class LauncherPage;

class LauncherApp : public BritepadApp {
  public:
    LauncherApp();
//    void init();
    void begin();
    void run();
    void end();
    bool event(Event* key);

    bool usesKeyboard() { return true; }
    BritepadApp* exitsTo() override { return A_SCREENSAVER_APP; }
    const char* name() { return "Launcher"; };

    AppType getAppType() { return NO_APP_TYPE; } // doesn't show a button for itself

    appid_t id() { return ID; };
    static constexpr appid_t ID = "laun";

    const char* statusBarTitle();
    const char* infoBarText();

    bool disablesScreensavers();

    void pushPage(direction_t d);
    void goToPage(pageindex_t s);

  private:
    const static int noButton = -1;

    const static int resetScreenTimeout = 10;  // seconds

    LauncherPage* getCurrentPage();
    pageindex_t getCurrentPageIndex() { return current_page; }
    void setCurrentPageIndex(pageindex_t n);
    void setCurrentPageIndex();
    pageindex_t firstPageOfType(AppType type);
    pageindex_t pageCount();

    color_t bgColor();

    void makePages();
    void freePages();

    pageindex_t current_page;


    time_t lastRun = 0;
    time_t lastBegin = 0;
    bool audibleExit = false;
    bool waitForRelease = false;

    const AppType defaultPageAppType = KEY_APP;
    String searchString;

    LauncherPage* _pages = nullptr;
};

#endif

