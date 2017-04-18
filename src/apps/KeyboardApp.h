#ifndef _KeyboardApp_
#define _KeyboardApp_

#include "BritepadApp.h"

class KeyboardApp : public BritepadApp {
public:
    AppType getAppType() { return SETTINGS_APP; }
};

#endif // _KeyboardApp_