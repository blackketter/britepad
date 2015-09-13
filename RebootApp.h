#ifndef _RebootApp_
#define _RebootApp_

#include "BritepadApp.h"
#include "BritepadShared.h"
#include "Sound.h"

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

class RebootApp : public BritepadApp {

  public:
    RebootApp(void) {};

    BritepadApp* run(void) {

      while (1) {
        // todo - make this teensyduino aware
        // todo - make this work
        CPU_RESTART
      };

      return BACK_APP;
    }

    bool disabled(void) { return true; }; // this doesn't work now

    const char* name(void) { return "Reboot"; };

    bool isInvisible(void) { return true; };

    appid_t id() { return ID; };
    static constexpr appid_t ID = "boot";

};

#endif


