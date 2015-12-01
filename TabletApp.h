#ifndef _TabletApp_
#define _TabletApp_

#include "BritepadApp.h"

class TabletApp : public BritepadApp {
  public:
    void begin() { mouse.setAbsolute(true); BritepadApp::begin(); }
    void end() {  mouse.setAbsolute(false); BritepadApp::end(); }

    void run() {
      BritepadApp::run();
      if (pad.touched(SCREEN_PAD)) {
          screen.fillCircle(pad.x(), pad.y(), penRadius, currentColor++);
      }
    }

    // interactive = mouse mode for this app
    void setAppMode(AppMode newMode) { if (newMode == INTERACTIVE_MODE) { newMode = MOUSE_MODE; } BritepadApp::setAppMode(newMode); }

    bool disablesScreensavers() { return true; }

    const char* name() { return "Tablet"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "tabl";

  private:
    int currentColor = screen.red;
    static const coord_t penRadius = 5;
};

#endif
