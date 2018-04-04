#include "BritepadApp.h"

class TabletApp : public BritepadApp {
  public:
    void begin(AppMode asMode) { mousePad.setAbsolute(true); BritepadApp::begin(asMode); switchAppMode(MOUSE_MODE); }
    void end() {  mousePad.setAbsolute(false); BritepadApp::end(); }

    void run() {
      BritepadApp::run();
      if (pad.touched(SCREEN_PAD)) {
          screen.fillCircle(pad.x(), pad.y(), penRadius, currentColor++);
      }
    }

    bool canBeMouse() { return isCurrentApp(); }  // switch to mouse mode only if we're already running (interactively)

    // interactive = mouse mode for this app
    void switchAppMode(AppMode newMode) { if (newMode == INTERACTIVE_MODE) { newMode = MOUSE_MODE; } BritepadApp::switchAppMode(newMode); }
    AppType getAppType() { return INTERACTIVE_APP; }

    bool disablesScreensavers() { return true; }

    const char* name() { return "Tablet"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "tabl";

  private:
    int currentColor = screen.red;
    static const coord_t penRadius = 5;
};

TabletApp theTabletApp;