#ifndef _KeyMatrix_
#define _KeyMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "KeyLayout.h"
#include "ErgodoxLayout.h"

class KeyMatrix {
  public:
    KeyMatrix();
    void begin();

    void setLayout(const keylayout_t* l = nullptr);  // pass nullptr to reset to default layout
    const keylayout_t* getLayout() { return currentLayout; }
    const keylayout_t* getDefaultLayout() { return defaultLayout; }

    keyswitch_t update();  //returns number of keys changed
    void sendKeys();  // send key events to host

    keyswitch_t numKeys() { return numColumns * numRows; }  // returns the total number of keys in the matrix
    uint8_t getWidth();
    uint8_t getHeight();

    // is there a physical key at this location, if so return its keylayout_t, pass null for current layout
    const keylayout_t* getKey(keyswitch_t k, const keylayout_t* l = nullptr);

    uint8_t getKeyWidth(keyswitch_t k);
    uint8_t getKeyHeight(keyswitch_t k);
    uint8_t getKeyX(keyswitch_t k);
    uint8_t getKeyY(keyswitch_t k);
    char getChar(keyswitch_t k);

    // returns the number of keys that changed state in the last idle
    keyswitch_t keysPressed();
    keyswitch_t keysReleased();

    // did a given key change?
    inline bool keyChanged(keyswitch_t k) { return (k != NO_KEY) && ((changedKeys[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool keyChanged(keycode_t c) { return (c != NO_CODE) && keyChanged(getSwitch(c)); }
    inline bool keyPressed(keyswitch_t k) { return keyChanged(k) && isKeyDown(k); }
    inline bool keyReleased(keyswitch_t k) { return keyChanged(k) && isKeyUp(k); }
    inline bool keyPressed(keycode_t c) { keyswitch_t k = getSwitch(c); return (c!=NO_CODE) && isKeyDown(k) && keyChanged(k); }
    inline bool keyReleased(keycode_t c) { keyswitch_t k = getSwitch(c); return (c!=NO_CODE) && isKeyUp(k) && keyChanged(k); }

    // marks key as not changed so that the event is not processed
    inline void clearKeyChange(keyswitch_t k) { changedKeys[k/numRows] = changedKeys[k/numRows] & ~(0x01 << (k%numRows)); }
    inline void clearKeyChange(keycode_t c) { clearKeyChange(getSwitch(c)); }
    void clearKeyChanges();

    keycode_t getCode(keyswitch_t k);
    keyswitch_t getSwitch(keycode_t c);

    inline bool isKeyDown(keyswitch_t k) { return ((curState[k/numRows] >> (k%numRows)) & 0x01); }
    inline bool isKeyUp(keyswitch_t k) { return !isKeyDown(k); }

    inline bool isKeyDown(keycode_t c) { return isKeyDown(getSwitch(c)); }
    inline bool isKeyUp(keycode_t c) { return !isKeyDown(c); }

    const keyinfo_t* getKeyInfo(keycode_t c);
    char getKeyChar(keycode_t c);
    const icon_t getKeyIcon(keycode_t c);
    const char* getKeyLabel(keycode_t c);

    millis_t getHistoryTime(uint8_t n);
    keyswitch_t getHistoryKey(uint8_t n);
    keycode_t getHistoryCode(uint8_t n);
    bool getHistoryPressed(uint8_t n);
    bool getHistoryReleased(uint8_t n) { return !getHistoryPressed(n); }
    void addHistory(keyswitch_t k, millis_t t, bool d);
    void clearHistory();

    bool doubleTapped(keyswitch_t k);
    bool doubleTapped(keycode_t c);

    void dumpStatus();
  private:
    millis_t lastScan = 0;
    static const millis_t minScanInterval = 5;
    static const millis_t doubleTapTime = 500;

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
    const keylayout_t* defaultLayout = ergodoxLayout;

    keyswitch_t keysChanged();
    static const uint8_t historySize = 10;
    millis_t historyTime[historySize];
    keyswitch_t historyKey[historySize];
    keycode_t historyCode[historySize];
    bool historyPressed[historySize];
};
#endif