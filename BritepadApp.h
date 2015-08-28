#ifndef _BritepadApp_
#define _BritepadApp_

#include "Britepad.h"
#include "Debug.h"

#define STAY_IN_APP ((BritepadApp*)0)
#define DEFAULT_APP ((BritepadApp*)1)
#define BACK_APP ((BritepadApp*)2)
#define DONT_EXIT_APP ((BritepadApp*)nil)

// todo move this into the base object
#define BritepadApp_SCRATCH_PAD_SIZE ((long)32768)
extern uint8_t BritepadApp_SCRATCH_PAD[];

class BritepadApp {
  public:
    BritepadApp() {};
    virtual void begin(void) { screen.fillScreen(screen.black); };
    virtual void end(void) {};
    virtual BritepadApp* run(void) {return nil;};
    virtual const char* name(void) { return 0; };
    virtual color_t buttonColor(void) { return screen.blue; }

    virtual bool isScreensaver(void) { return false; };
    virtual bool isPopup(void) { return false; };        // popup apps don't need begin or end, call run() just once
    virtual bool isInvisible(void) { return false; };    // has no UI
};

#endif


