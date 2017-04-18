#ifndef _LaunchBarApp_
#define _LaunchBarApp_

#include "KeyboardApp.h"

class LaunchBarApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "LaunchBar"; };

    void idle();
  private:
    static const millis_t releaseTimeout = 1000;
};

#endif // _LaunchBarApp_