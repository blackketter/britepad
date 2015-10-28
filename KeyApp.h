#ifndef _KeyApp_
#define _KeyApp_

#include "BritepadApp.h"
#include "BritepadShared.h"

class KeyApp : public BritepadApp {

  public:
    KeyApp(const char* name, const char* keys, color_t color = screen.blue) { nameStr = name; keyStr = keys; button_color = color; };
    KeyApp(const char* name, uint8_t key, color_t color = screen.blue ) { nameStr = name; mediaKey = key; button_color = color; }

    void run();
    const char* name() { return nameStr; };
    bool isPopup() { return true; };
    bool isInvisible();

    color_t buttonColor();

    appid_t id() { return ID; };
    static constexpr appid_t ID = "keys";

  protected:
    const char* nameStr;
    const char* keyStr;
    uint8_t  mediaKey = 0;
    color_t button_color = 0;;
    virtual void draw();
};

#endif
