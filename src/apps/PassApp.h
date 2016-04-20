#ifndef _PassApp_
#define _PassApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "KeyApp.h"
#include "Screen.h"

class PassApp : public KeyApp {

  public:
    PassApp(const char* name, const char* keys, color_t color, uint32_t pos) : KeyApp(name, keys, color, pos) {};
    PassApp(const char* name, uint8_t mediaKey, color_t color, uint32_t pos) : KeyApp(name, mediaKey, color, pos) {};
    color_t buttonColor() { return screen.red; };
    void draw();

    AppType getAppType() { return KEY_APP; }
    appid_t id() { return ID; };
    static constexpr appid_t ID = "pass";
};

#endif
