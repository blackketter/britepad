#ifndef _USBKeyMatrix_
#define _USBKeyMatrix_

#include "KeyMatrix.h"

class USBKeyMatrix : public KeyMatrix {
  public:
    USBKeyMatrix();
    void begin();
    keyswitch_t update();
    bool switchIsDown(keyswitch_t k);
    bool switchIsUp(keyswitch_t k);

  protected:

  private:

};

#endif // _USBKeyMatrix_