#include "DotsDisplayApp.h"

class BriteLiteApp : public DotsDisplayApp {

  public:
    void run();
    void begin();

    static constexpr appid_t ID = "lite";
    appid_t id() { return ID; };
    const char* name() { return "LiteBrite"; };

  protected:
    int getDotsWide() { return 16; }
    int getDotsHigh() { return 14; }

  private:
    int cursor_x;
    int cursor_y;
};
