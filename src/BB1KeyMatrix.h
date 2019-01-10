#ifndef _BB1KeyMatrix_
#define _BB1KeyMatrix_

#include "KeyMatrix.h"

class BB1KeyMatrix : public KeyMatrix {
  public:
    BB1KeyMatrix();
    void begin();
    void update();
    const char* name() { return "BB1"; }

  private:
    void scanMatrix();
    void clearKeyChanges();

    bool switchIsDown(keyswitch_t k) { return ((_curState[k/_numRows] >> (k%_numRows)) & 0x01); }
    bool switchIsUp(keyswitch_t k) { return !switchIsDown(k); }


    static const uint8_t _numRows = 5;
    static const uint8_t _numColumns = 17;

    const  pinNumber _columnPin[_numColumns] = {
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

    const  pinNumber _rowPin[_numRows] = {
      4,
      3,
      2,
      1,
      0
    };

    uint8_t _curState[_numColumns];  // assumes less than 8 rows
    uint8_t _lastState[_numColumns];
    uint8_t _changedKeys[_numColumns];

};

#endif
