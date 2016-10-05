#ifndef _ChromaClockApp_
#define _ChromaClockApp_

#include "AnalogClockApp.h"

class ChromaClockApp : public AnalogClockApp {
  public:
    void begin(AppMode asMode);
    void update();

    const char* name() { return "Chroma"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "chrm";

  private:
    void drawSlice(int index, color_t c, coord_t r);
    const int slices = 180;
};

#endif



