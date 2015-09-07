#ifndef _KeyApp_
#define _KeyApp_

#include "BritepadApp.h"
#include "BritepadShared.h"

class KeyApp : public BritepadApp {

  public:
    KeyApp(const char* name, const char* keys, color_t color = screen.blue) { nameStr = name; keyStr = keys; button_color = color; };
    KeyApp(const char* name, uint8_t key, color_t color = screen.blue ) { nameStr = name; mediaKey = key; button_color = color; }

    BritepadApp* run(void);
    const char* name(void) { return nameStr; };
    bool isPopup(void) { return true; };
    bool isInvisible(void);

    color_t buttonColor(void);

  protected:
    const char* nameStr;
    const char* keyStr;
    uint8_t  mediaKey = 0;
    color_t button_color = 0;;
    virtual void draw(void);
};

#endif
