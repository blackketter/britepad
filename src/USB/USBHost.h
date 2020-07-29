#ifndef _USBHost_
#define _USBHost_

#if defined(__MK66FX1M0__) || defined(__IMXRT1062__)

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
