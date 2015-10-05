#ifndef _PAINTAPP_
#define _PAINTAPP_

#include "BritepadApp.h"

class PaintApp : public BritepadApp {
  public:
    BritepadApp* run(void);

    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }

    const char* name(void) { return "Paint"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "pain";

  private:
    int currentColor = screen.red;
};

#endif
