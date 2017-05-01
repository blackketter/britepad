#ifndef _ChromaClockApp_
#define _ChromaClockApp_

#include "AnalogClockApp.h"

class ChromaClockApp : public AnalogClockApp {
  public:
    void begin(AppMode asMode);
    void update();
    void run();

    const char* name() { return "Chroma"; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "chrm";

  private:
    void drawSlice(int index, color_t c, coord_t r);
    static const int _slices = 180;
    int _curSlice = 0;
    millis_t _maxDrawTime = 10;
    coord_t r[_slices];
    color_t c[_slices];
};

#endif



