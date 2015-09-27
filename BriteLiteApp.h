#include "DotsDisplayApp.h"

class BriteLiteApp : public DotsDisplayApp {

  public:
    void begin(bool asScreensaver) { ScreensaverApp::begin(asScreensaver); dots.init(16,14, BritepadAppScratchPad); };
    BritepadApp* run(void);

    static constexpr appid_t ID = "lite";
    appid_t id() { return ID; };
    const char* name(void) { return "LiteBrite"; };


  private:
    HexDotMatrix dots;
    int cursor_x;
    int cursor_y;
    color_t lastColor;
};
