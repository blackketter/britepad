#ifndef _KeyboardMatrix_
#define _KeyboardMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "KeyboardLayout.h"
#include "ErgodoxLayout.h"

class KeyboardMatrix {
  public:
    KeyboardMatrix();
    void begin();

    void setLayout(const keylayout_t* l = nullptr) { currentLayout = l ? l : getDefaultLayout(); }  // pass nullptr to reset to default layout
    const keylayout_t* getLayout() { return currentLayout; }
    const keylayout_t* getDefaultLayout() { return ergodoxLayout; }

    keyswitch_t update();  //returns number of keys changed
    void sendKeys();  // send key events to host

    keyswitch_t numKeys() { return numColumns * numRows; }  // returns the total number of keys in the matrix

    // returns the number of keys that changed state in the last idle
    keyswitch_t keysChanged();
    keyswitch_t keysPressed();
    keyswitch_t keysReleased();

    // did a given key change?
    inline bool keyChanged(keyswitch_t k) { return ((changedKeys[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool keyChanged(keycode_t c) { return keyChanged(getSwitchFromCode(c)); }
    inline bool keyPressed(keyswitch_t k) { return keyChanged(k) && isKeyDown(k); }
    inline bool keyReleased(keyswitch_t k) { return keyChanged(k) && isKeyUp(k); }
    inline bool keyPressed(keycode_t c) { keyswitch_t k = getSwitchFromCode(c); return isKeyDown(k) && keyChanged(k); }
    inline bool keyReleased(keycode_t c) { keyswitch_t k = getSwitchFromCode(c); return isKeyUp(k) && keyChanged(k); }

    // marks key as not changed so that the event is not processed
    inline void clearKeyChange(keyswitch_t k) { changedKeys[k/numRows] = changedKeys[k/numRows] & ~(0x01 << (k%numRows)); }
    inline void clearKeyChange(keycode_t c) { clearKeyChange(getSwitchFromCode(c)); }
    void clearKeyChanges();

    keycode_t getCodeFromSwitch(keyswitch_t k);
    keyswitch_t getSwitchFromCode(keycode_t c);

    inline bool isKeyDown(keyswitch_t k) { return ((curState[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool isKeyUp(keyswitch_t k) { return !isKeyDown(k); }

    inline bool isKeyDown(keycode_t c) { return isKeyDown(getSwitchFromCode(c)); }
    inline bool isKeyUp(keycode_t c) { return !isKeyDown(c); }

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
    uint8_t changedKeys[numColumns];

    void scanMatrix();
    const keylayout_t* currentLayout;
};
#endif