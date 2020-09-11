#ifndef _USBHost_
#define _USBHost_

#ifndef NOUSBHOST
#include "Arduino.h"
#include "USBHost_t36.h"

#else

class USBHost {
  public:
    void begin() {};
    void end() {};
    void Task() {};
};

#endif // __MK66FX1M0__

extern USBHost usbHost;

#endif // _USBHost_
