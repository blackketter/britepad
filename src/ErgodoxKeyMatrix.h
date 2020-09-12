#ifndef _ErgodoxKeyMatrix_
#define _ErgodoxKeyMatrix_

#include "KeyMatrix.h"
#include "MCP23018.h"

class ErgodoxKeyMatrix : public KeyMatrix {
  public:
    ErgodoxKeyMatrix();
    void begin(KeyEventQueue* queue) override;
    boolean update();
    const char* name() { return "ErgoDox"; }

  private:
    void scanMatrix();
    void clearKeyChanges();

    bool switchIsDown(keyswitch_t k) { return ((_curState[k/_numRows] >> (k%_numRows)) & 0x01); }
    bool switchIsUp(keyswitch_t k) { return !switchIsDown(k); }

    static const uint8_t _leftAddr = 0;
    static const uint8_t _rightAddr = 7;

    MCP23018 _leftMatrix; // columns 0-6
    MCP23018 _rightMatrix; // columns 7-13

    static const uint8_t _numRows = 6;
    static const uint8_t _numColumnsPerMatrix = 7;
    static const uint8_t _numColumns = _numColumnsPerMatrix*2;

    uint8_t _curState[_numColumns];  // assumes less than 8 rows
    uint8_t _lastState[_numColumns];
    uint8_t _changedKeys[_numColumns];

};

#endif
