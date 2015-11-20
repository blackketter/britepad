#ifndef _PassApp_
#define _PassApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "KeyApp.h"
#include "Screen.h"

class PassApp : public KeyApp {

  public:
    PassApp(const char* name, const char* keys, color_t color = screen.red) : KeyApp(name, keys, color) {};
    PassApp(const char* name, uint8_t mediaKey, color_t color = screen.red) : KeyApp(name, mediaKey, color) {};
    color_t buttonColor() { return screen.red; };
    void draw();

    appid_t id() { return ID; };
    static constexpr appid_t ID = "pass";
};

#endif
