#ifndef _USBKeyMatrix_
#define _USBKeyMatrix_

#include "KeyMatrix.h"
#include "USB/USBHost.h"

class USBKeyMatrix : public KeyMatrix {
  public:
    USBKeyMatrix();
    void begin(EventQueue* queue) override;
    bool update();
    const char* name() { return "USB"; }
    void usbEvent(keyswitch_t k, bool down);
  protected:

  private:
    bool _updated = false;
};

#endif // _USBKeyMatrix_
