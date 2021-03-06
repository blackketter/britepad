#ifndef _KeyApp_
#define _KeyApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "widgets/Icon.h"
#include "widgets/AppButton.h"

typedef int mediakey_t;

class KeyApp : public BritepadApp {

  public:
    KeyApp(const char* name, const char* keys, color_t color, uint32_t pos) { nameStr = name; keyStr = keys; button_color = color; setLauncherPosition(pos);};
    KeyApp(const char* name, mediakey_t key, color_t color, uint32_t pos) { nameStr = name; mediaKey = key; button_color = color; setLauncherPosition(pos);}
    KeyApp(icon_t i, mediakey_t key, color_t color, uint32_t pos) { _icon = i; mediaKey = key; button_color = color; setLauncherPosition(pos);}

    void begin() {
      // if this isn't a repeat, then reset the time
      if (!pad.touched(SCREEN_PAD)) {
        lastKey = 0;
      }
      BritepadApp::begin();
    }
    void run();

    const char* name() { return nameStr; };
    bool canBeInvisible();
    BritepadApp* exitsTo() override { return A_SCREENSAVER_APP; }

    appid_t id() { return ID; };
    static constexpr appid_t ID = "1key";

    AppType getAppType() { return KEY_APP; }

    color_t appButtonColor() { return button_color; }
    Icon getIcon() { return _icon; }

  protected:

    const char* nameStr = "";
    const char* keyStr = nullptr;
    mediakey_t  mediaKey = 0;
    color_t button_color = 0;;
    virtual void draw();

    millis_t lastKey = 0;
    bool repeating = false;
    const millis_t keyRepeat = 125;
    const millis_t keyStartRepeat = 1000;
    Icon _icon;
    };

#endif
