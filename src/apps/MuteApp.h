#ifndef _MuteApp_
#define _MuteApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

class MuteApp : public BritepadApp {

  public:
    void init() {
      uint8_t mutestate;
      prefs.read(id(), sizeof(mutestate), &mutestate);
      sound.setMute(mutestate);
    };

    void run() {
      sound.setMute(!sound.getMute());
      if (!sound.getMute()) {
        sound.beep();
      }
      uint8_t mutestate = sound.getMute();
      prefs.write(id(), sizeof(mutestate), &mutestate);
    }

    const char* name() { return sound.getMute() ? "Pad Muted" : "Mute Pad"; };

    bool isPopup() { return true; };

    bool isInvisible() { return true; };
    AppType getAppType() { return SETTINGS_APP; }

    color_t buttonColor() { return sound.getMute() ? screen.darkblue : screen.blue; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "mute";

};

#endif

