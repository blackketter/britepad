#ifndef _KeyboardApp_
#define _KeyboardApp_

#include "BritepadApp.h"

class KeyboardApp : public BritepadApp {
public:
    AppType getAppType() { return KEYBOARD_APP; }
};

#endif // _KeyboardApp_