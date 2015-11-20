#ifndef _FadeDotsApp_
#define _FadeDotsApp_

#include "DotsDisplayApp.h"

class FadeDotsApp : public DotsDisplayApp {
  public:
    void run();

    const char* name() { return "Fade Dots"; };
    static constexpr appid_t ID = "fade";
    appid_t id() { return ID; };

  protected:
};

#endif


