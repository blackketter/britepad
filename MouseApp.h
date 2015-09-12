#ifndef _MOUSEAPP_
#define _MOUSEAPP_

#include "BritepadApp.h"

class MouseApp : public BritepadApp {
  public:
    MouseApp(void) : BritepadApp() {  Mouse.begin(); };
    BritepadApp* run(void);
    void end(BritepadApp* nextApp);
    const char* name(void) { return "Mouse"; };

    appid_t id() { return ID; }
    static constexpr appid_t ID = "mous";

  private:
    int currentColor = screen.red;
    int backgroundColor = screen.black;
    bool scrollMode = false;
};

#endif
