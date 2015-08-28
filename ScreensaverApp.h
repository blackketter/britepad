#ifndef _ScreensaverApp_
#define _ScreensaverApp_

#include "Britepad.h"
#include "BritepadApp.h"

extern BritepadApp* currentScreensaver;

class ScreensaverApp : public BritepadApp {

  public:
    virtual bool isScreensaver(void) { return true; };
    color_t buttonColor(void) { return (this == currentScreensaver ? screen.yellow : screen.darkyellow); };
};



#endif

