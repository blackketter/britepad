#ifndef _KeyApp_
#define _KeyApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "widgets/Icon.h"

typedef int mediakey_t;

class KeyApp : public BritepadApp {

  public:
    KeyApp(const char* name, const char* keys, color_t color, uint32_t pos) { nameStr = name; keyStr = keys; button_color = color; setLauncherPosition(pos);};
    KeyApp(const char* name, mediakey_t key, color_t color, uint32_t pos) { nameStr = name; mediaKey = key; button_color = color; setLauncherPosition(pos);}
    KeyApp(icon_t i, mediakey_t key, color_t color, uint32_t pos) { icon = i; mediaKey = key; button_color = color; setLauncherPosition(pos);}

    void run();
    void begin(AppMode asMode) { lastKey = 0; BritepadApp::begin(asMode); }

    const char* name() { return nameStr; };
    bool isInvisible();

    color_t buttonColor();

    appid_t id() { return ID; };
    static constexpr appid_t ID = "keys";

    AppType getAppType() { return KEY_APP; }

  protected:

    const char* nameStr = nullptr;
    const char* keyStr = nullptr;
    mediakey_t  mediaKey = 0;
    color_t button_color = 0;;
    virtual void draw();

    millis_t lastKey = 0;
    bool repeating = false;
    const millis_t keyRepeat = 250;
    const millis_t keyStartRepeat = 1000;
};

#endif
