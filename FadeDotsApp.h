#ifndef _FadeDotsApp_
#define _FadeDotsApp_

#include "DotsDisplayApp.h"

class FadeDotsApp : public DotsDisplayApp {
  public:
    void begin(AppMode asMode);
    BritepadApp* run(void);

    const char* name(void) { return "Fade Dots"; };
    static constexpr appid_t ID = "fade";
    appid_t id() { return ID; };
};

#endif


