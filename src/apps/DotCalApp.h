#ifndef _DotCalApp_
#define _DotCalApp_

#include "ClockApp.h"
#include "BritepadShared.h"
#include "widgets/DotMatrix.h"
#include "Screen.h"

class DotCalApp : public ClockApp {
  public:
    void begin(AppMode asMode);
    void update();
    void end();

    const char* name() { return "Dot Cal"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "dcal";
    virtual bool displaysClock() { return false; };

  protected:
    virtual int getDotsWide() { return 31; }
    virtual int getDotsHigh() { return 12; }
    DotMatrix* dots = nullptr;

    const color_t beforeColor = screen.white;
    const color_t afterColor = screen.grey;
    const color_t blankColor = screen.black;
};

#endif


