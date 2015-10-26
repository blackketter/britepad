#include "DotsDisplayApp.h"

class BriteLiteApp : public DotsDisplayApp {

  public:
    void begin(AppMode asMode) { ScreensaverApp::begin(asMode); dots.init(16,14, (color_t*)dotData); };
    void run(void);

    static constexpr appid_t ID = "lite";
    appid_t id() { return ID; };
    const char* name(void) { return "LiteBrite"; };



  private:
    color_t dotData[16][14];
    HexDotMatrix dots;
    int cursor_x;
    int cursor_y;
};
