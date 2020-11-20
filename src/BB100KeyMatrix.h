#ifndef _BB100KeyMatrix_
#define _BB100KeyMatrix_

#include "KeyMatrix.h"

class BB100KeyMatrix : public KeyMatrix {
  public:
    const char* name() override { return "BB-100 Key Matrix"; }
    bool update() override { return false;}
};
#endif
