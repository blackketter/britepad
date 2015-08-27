#ifndef _BPApp_
#define _BPApp_

#include "Britepad.h"
#include "Debug.h"

#define DEFAULT_APP ((BPApp*)1)
#define BACK_APP ((BPApp*)2)
#define DONT_EXIT_APP ((BPApp*)nil)

// todo move this into the base object
#define BPAPP_SCRATCH_PAD_SIZE ((long)32768)
extern uint8_t BPAPP_SCRATCH_PAD[];

class BPApp {
  public:
    BPApp() {};
    virtual void begin(void) { screen.fillScreen(screen.black); };
    virtual void end(void) {};
    virtual BPApp* run(void) {return nil;};
    virtual const char* name(void) { return 0; };
    virtual color_t buttonColor(void) { return screen.blue; }

    virtual bool isScreensaver(void) { return false; };
    virtual bool isPopup(void) { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible(void) { return false; };    // has no UI
};

#endif


