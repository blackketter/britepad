#ifndef _InfoApp_
#define _InfoApp_
#include "BritepadApp.h"

class InfoApp : public BritepadApp {
  public:
    void run();
    const char* name() { return "Info"; };
    bool displaysStatusBar() { return true; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return true; }
    BritepadApp* exitsTo() { return DEFAULT_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "info";

  private:
    time_t lastUpdate = 0;
};


#endif
