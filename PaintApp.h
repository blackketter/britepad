#ifndef _PAINTAPP_
#define _PAINTAPP_

#include "BritepadApp.h"

class PaintApp : public BritepadApp {
  public:
    void run();

    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }

    const char* name() { return "Paint"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "pain";

  private:
    int currentColor = screen.red;
    static const coord_t penRadius = 10;
};

#endif
