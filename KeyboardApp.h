#ifndef _KeyboardApp_
#define _KeyboardApp_

#include "BritepadApp.h"
#include "TouchKeyboard.h"

class KeyboardApp : public BritepadApp {
  public:
    void run();
    void begin();
    const char* name() { return "Keyboard"; };
    bool displaysStatusBar() { return true; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return true; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "keyb";

  private:
    TouchKeyboard kbd;
};

#endif



