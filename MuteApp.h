#ifndef _MuteApp_
#define _MuteApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

class MuteApp : public BritepadApp {

  public:
    MuteApp(void) {
      uint8_t mutestate;
      prefs.read(id(), sizeof(mutestate), &mutestate);
      sound.setMute(mutestate);
    };

    BritepadApp* run(void) {
      sound.setMute(!sound.getMute());
      if (!sound.getMute()) {
        sound.beep();
      }
      uint8_t mutestate = sound.getMute();
      prefs.write(id(), sizeof(mutestate), &mutestate);
      return STAY_IN_APP;
    }

    const char* name(void) { return sound.getMute() ? "Pad Muted" : "Mute Pad"; };

    bool isPopup(void) { return true; };

    bool isInvisible(void) { return true; };

    color_t buttonColor(void) { return sound.getMute() ? screen.darkblue : screen.blue; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "mute";

};

#endif

