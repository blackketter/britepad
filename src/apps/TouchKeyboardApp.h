#ifndef _TouchKeyboardApp_
#define _TouchKeyboardApp_

#include "BritepadApp.h"
#include "widgets/TouchKeyboard.h"

class TouchKeyboardApp : public BritepadApp {
  public:
    void run();
    void begin(AppMode asMode);
    void end();
    const char* name() { return "Keyboard"; };
    bool displaysStatusBar() { return true; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "keyb";

    AppType getAppType() { return KEY_APP; }

  private:
    TouchKeyboard* kbd;
};

#endif



