#include "Hardware.h"

#ifdef TEENSYDUINO
extern volatile uint8_t usb_configuration;

bool usbActive() {
  return usb_configuration;
};

#else
bool usbActive() { return false; }
#endif
