#ifndef _KeyboardMatrix_
#define _KeyboardMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "keylayouts.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;

typedef uint16_t keycode_t;
const keycode_t NO_CODE = 0;

typedef struct keymap_t {
  keyswitch_t key;
  keycode_t code;
  uint8_t x;
  uint8_t y;
} keymap_t;

class KeyboardMatrix {
  public:
    KeyboardMatrix();
    void begin();

    keyswitch_t update();  //returns number of keys changed
    void sendKeys();  // send key events to host

    inline bool isKeyDown(keyswitch_t k) { return ((curState[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool wasKeyDown(keyswitch_t k) { return ((lastState[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool isKeyUp(keyswitch_t k) { return !isKeyDown(k); }
    inline bool wasKeyUp(keyswitch_t k) { return !wasKeyDown(k); }
    inline bool didKeyChange(keyswitch_t k) { return isKeyDown(k) != wasKeyDown(k); }

    keyswitch_t numKeys() { return numColumns * numRows; }  // returns the total number of keys in the matrix
    keyswitch_t keysChanged(); // returns the number of keys that changed state in the last idle
    keyswitch_t keyChanged(keyswitch_t n); // returns the nth key that's changed (up to the number returned by keysChanged())

    keycode_t getCodeFromSwitch(keyswitch_t k);
  private:
    millis_t lastScan = 0;
    static const millis_t minScanInterval = 5;

    static const uint8_t numRows = 6;
    static const uint8_t numColumnsPerMatrix = 7;
    static const uint8_t numColumns = numColumnsPerMatrix*2;

    static const uint8_t leftAddr = 0;
    static const uint8_t rightAddr = 7;

    MCP23018 leftMatrix; // columns 0-6
    MCP23018 rightMatrix; // columns 7-13

    uint8_t curState[numColumns];  // assumes less than 8 rows
    uint8_t lastState[numColumns];
    void scanMatrix();
    const keymap_t* currentLayout;
};

class KeyMap {
  public:
    KeyMap();
    char getChar(keyswitch_t k);
};
#endif