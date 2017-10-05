#ifndef _KeyboardViewerApp_
#define _KeyboardViewerApp_
#include "BritepadApp.h"
#include "KeyboardApp.h"
#include "widgets/KeyboardWidget.h"


class KeyboardViewerApp : public KeyboardApp {
  public:
    KeyboardViewerApp(KeyMatrix* keyMatrix) { _keyMatrix = keyMatrix; };
    void begin(AppMode asMode);
    void end();
    void run();
    void event(KeyEvent* key);

    const char* name() { _name = _keyMatrix->name(); _name.append(" Keys");  return _name.c_str(); };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "keys";

    bool disablesScreensavers() { return true; }
    bool canBeInteractive() { return true; }

    // appear as a setting on the keyboard screen and on the main screen
    AppType getAppType() { return (AppType)(KEYBOARD_APP | KEY_APP); }


  private:
    void draw();
    KeyboardWidget* buttons = nullptr;
    bool tutorialMode = false;
    BritepadApp* lastApp = nullptr;
    KeyMatrix* _keyMatrix = nullptr;
    String _name;
    AppMode lastMode;
};

#endif


