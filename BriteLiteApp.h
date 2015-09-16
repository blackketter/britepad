#include "DotsDisplayApp.h"

class BriteLiteApp : public DotsDisplayApp {

  public:
    void begin(void) { ScreensaverApp::begin(); setUpDots(16,14); };
    BritepadApp* run(void);

    static constexpr appid_t ID = "lite";
    appid_t id() { return ID; };
    const char* name(void) { return "LiteBrite"; };


  protected:
    virtual void updateDot(int x, int y);

  private:
    int cursor_x;
    int cursor_y;
    color_t lastColor;
};
