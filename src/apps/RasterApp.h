#ifndef _RasterApp_
#define _RasterApp_

#include "ScreensaverApp.h"

class RasterApp : public ScreensaverApp {
  public:
    void run();
    void begin();
    bool canBeMouse() { return false; }
    bool canBeInteractive() { return true; }

    const char* name() { return "Raster"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "rast";

  private:
};

#endif
