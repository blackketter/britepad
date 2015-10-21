#ifndef _InfoApp_
#define _InfoApp_
#include "BritepadApp.h"

class InfoApp : public BritepadApp {
  public:
    void run(void);
    const char* name(void) { return "Info"; };
    bool displaysStatusBar(void) { return true; }
    bool canBeInteractive() { return true; }
    bool disablesScreensavers() { return true; }
    BritepadApp* exitsTo(void) { return DEFAULT_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "info";

  private:
    time_t lastUpdate = 0;
};


#endif
