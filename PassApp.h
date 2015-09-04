#ifndef _PassApp_
#define _PassApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "KeyApp.h"
#include "Screen.h"

class PassApp : public KeyApp {

  public:
    PassApp(const char* name, const char* keys, color_t color = screen.red) : KeyApp(name, keys, color) {};
    PassApp(const char* name, uint8_t key, color_t color = screen.red) : KeyApp(name, key, color) {};
    color_t buttonColor(void) { return screen.red; };
    void draw(void);
};

#endif
