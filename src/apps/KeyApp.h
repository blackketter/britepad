#ifndef _KeyApp_
#define _KeyApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "widgets/Icon.h"

class KeyApp : public BritepadApp {

  public:
    KeyApp(const char* name, const char* keys, color_t color = screen.blue) { nameStr = name; keyStr = keys; button_color = color; };
    KeyApp(const char* name, uint8_t key, color_t color = screen.blue ) { nameStr = name; mediaKey = key; button_color = color; }
    KeyApp(icon_t i, uint8_t key, color_t color = screen.blue ) { icon = i; mediaKey = key; button_color = color; }

    void run();
    const char* name() { return nameStr; };
     bool isPopup() { return true; };
    bool isInvisible();

    color_t buttonColor();

    appid_t id() { return ID; };
    static constexpr appid_t ID = "keys";

    AppType getAppType() { return KEY_APP; }


  protected:
    const char* nameStr = nullptr;

    const char* keyStr = nullptr;
    uint8_t  mediaKey = 0;
    color_t button_color = 0;;
    virtual void draw();
};

#endif
