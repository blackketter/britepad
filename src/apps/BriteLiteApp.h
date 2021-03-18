#ifndef _BriteLiteApp_
#define _BriteLiteApp_

#include "DotsDisplayApp.h"

class BriteLiteApp : public DotsDisplayApp {

  public:
    void run();
    void begin();

    static constexpr appid_t ID = "lite";
    appid_t id() { return ID; };
    const char* name() { return "LiteBrite"; };

  protected:
    int getDotsWide() { return screen.width() / 20; }
    int getDotsHigh() { return screen.height() / 16; }

  private:
    int cursor_x;
    int cursor_y;
};
#endif
