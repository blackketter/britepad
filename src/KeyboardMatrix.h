#ifndef _KeyboardMatrix_
#define _KeyboardMatrix_

#include "Clock.h"
#include "MCP23018.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;

class KeyboardMatrix {
  public:
    KeyboardMatrix();
    void begin();

    bool idle();

    bool isKeyDown(keyswitch_t k);
    bool wasKeyDown(keyswitch_t k);
    bool isKeyUp(keyswitch_t k) { return !isKeyDown(k); }
    bool wasKeyUp(keyswitch_t k) { return !wasKeyDown(k); }
    bool didKeyChange(keyswitch_t k) { return isKeyDown(k) != wasKeyDown(k); }

    keyswitch_t numKeys();  // returns the total number of keys in the matrix
    keyswitch_t keysChanged(); // returns the number of keys that changed state in the last idle
    keyswitch_t keyChanged(keyswitch_t n); // returns the nth key that's changed (up to the number returned by keysChanged())

  private:
    static const uint8_t numRows = 6;
    static const uint8_t numColumnsPerMatrix = 7;
    static const uint8_t numColumns = numColumnsPerMatrix*2;

    static const uint8_t leftAddr = 0;
    static const uint8_t rightAddr = 7;

    millis_t lastScan = 0;
    MCP23018 leftMatrix; // columns 0-6
    MCP23018 rightMatrix; // columns 7-13

    uint8_t curState[numColumns];  // assumes less than 8 rows
    uint8_t lastState[numColumns];
};

class KeyMap {
  public:
    KeyMap();
    char getChar(keyswitch_t k);
};
#endif