#ifndef _StarfieldApp_
#define _StarfieldApp_

#include "ScreensaverApp.h"

class StarfieldApp : public ScreensaverApp {
  public:
    void run();

    const char* name() { return "Stars"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "star";

    bool canBeMouse() { return true; }

  protected:
   void releaseMem();

  private:
    coord_t* stars = nullptr;
    uint8_t* distance = nullptr;
    static const int maxSpeed = 15;
    int randomDistance() { return random(maxSpeed) + 1; }
};

#endif
