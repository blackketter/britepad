#ifndef _LifeApp_
#define _LifeApp_

#include "DotsDisplayApp.h"

class LifeApp : public DotsDisplayApp {
  public:
    void begin(bool asScreensaver);
    BritepadApp* run(void);

    const char* name(void) { return "Life"; };
    static constexpr appid_t ID = "life";
    appid_t id() { return ID; };
};

#endif


