#ifndef _PercentApp_
#define _PercentApp_

#include "ClockApp.h"
#include "BritepadShared.h"
#include "widgets/DotMatrix.h"
#include "Screen.h"

class PercentApp : public ClockApp {
  public:
    void begin();
    void update();
    void end();

    const char* name() { return "Percent"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "perc";
    virtual bool displaysClock() { return false; };

  protected:
    virtual int getDotsWide() { return 12; }
    virtual int getDotsHigh() { return 8; }
    DotMatrix* dots = nullptr;

  private:
    int dotsNow();
    const color_t beforeColor = screen.white;
    const color_t afterColor = screen.grey;
    color_t nowColor() { return clock.now()%2 ? beforeColor : afterColor; }
};

#endif


