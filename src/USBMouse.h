#ifndef _USBMouse_
#define _USBMouse_

#include "BritepadShared.h"
#include "USBHost.h"

class USBMouse {
  public:
    void begin() {}
    void end() {}
    void run();

  private:
};

extern USBMouse usbMouse;

#endif
