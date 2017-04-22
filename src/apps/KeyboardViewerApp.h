#ifndef _KeyboardViewerApp_
#define _KeyboardViewerApp_
#include "BritepadApp.h"
#include "widgets/Button.h"
#include "widgets/Icons.h"
#include "widgets/WidgetGroup.h"


class KeyboardViewerApp : public BritepadApp {
  public:
    void begin(AppMode asMode);
    void end();
    void run();
    void idle();

    const char* name() { return "Keys"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "keys";
    AppType getAppType() { return INTERACTIVE_APP; }
    bool wantsToRun() { return tutorialMode; }
    bool disablesScreensavers() { return manuallyLaunched; }

  private:
    void draw();
    WidgetGroup* buttons = nullptr;
    bool tutorialMode = false;
    bool manuallyLaunched = false;
};

#endif


