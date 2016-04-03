#ifndef _MandelApp_
#define _MandelApp_

#include "BritepadApp.h"

class MandelApp : public BritepadApp {
  public:
    void run();
    void begin(AppMode asMode) { lastRow = screen.clipTop(); BritepadApp::begin(asMode); }
    bool canBeMouse() { return true; }
    bool canBeInteractive() { return true; }

    const char* name() { return "Mandel"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "mand";

  private:
    coord_t lastRow;
    float xOrigin = 0;
    float yOrigin = 0;
    float scale = 0;
};

#endif
