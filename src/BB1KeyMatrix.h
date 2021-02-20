#ifndef _BB1KeyMatrix_
#define _BB1KeyMatrix_
#include "BritepadShared.h"
#include "BB1Layout.h"

#include "GPIOKeyMatrix.h"

    const  pinNumber columnPins[] = {
      5,
      6,
      7,
      8,
      24,
      25,
      26,
      27,
      28,
      29,
      30,
      31,
      32,
      33,
      34,
      35,
      36
    };

    const  pinNumber rowPins[] = {
      4,
      3,
      2,
      1,
      0
    };


class BB1KeyMatrix : public GPIOKeyMatrix {
  public:
    BB1KeyMatrix() : GPIOKeyMatrix(BB1Layout, BB1Map, 5, 17, rowPins, columnPins) {};
    const char* name() { return "BB1"; }

};

#endif
