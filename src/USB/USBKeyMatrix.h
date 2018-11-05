#ifndef _USBKeyMatrix_
#define _USBKeyMatrix_

#include "KeyMatrix.h"
#include "USB/USBHost.h"

class USBKeyMatrix : public KeyMatrix {
  public:
    USBKeyMatrix();
    void begin();
    void update();
    const char* name() { return "USB"; }
    void usbKeyEvent(keyswitch_t k, bool down);
  protected:

  private:

};

#endif // _USBKeyMatrix_