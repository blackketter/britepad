#ifndef _KeyMatrix_
#define _KeyMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "Widgets/icon.h"
#include "KeyInfo.h"
#include "KeyLayout.h"

typedef struct keyevent_t {
    millis_t time;
    keyswitch_t key;
    keycode_t code;
    bool pressed;
} keyevent_t;

class KeyMatrix {
  public:
    KeyMatrix(const keymap_t* defaultMap, const keylayout_t* defaultLayout );
    void begin();

    void setMap(const keymap_t* m = nullptr);  // pass nullptr to reset to default map
    const keymap_t* getMap() { return _currentMap; }
    const keymap_t* getDefaultMap() { return _defaultMap; }

    const keyoverlay_t* getOverlay() { return _currentOverlay; }
    const void setOverlay(const keyoverlay_t* o = nullptr) { _currentOverlay = o; }
    keycode_t lookupOverlay(keycode_t c);

    void setLayout(const keylayout_t* l = nullptr);   // pass nullptr to reset to default layout
    const keylayout_t* getLayout() { return _currentLayout; }
    const keylayout_t* getDefaultLayout() { return _defaultLayout; }

    keyswitch_t update();  //returns number of keys changed
    keyswitch_t sendKeys();  // send key events to host, returns number of key events sent

    keyswitch_t numKeys() { return _numColumns * _numRows; }  // returns the total number of keys in the matrix
    uint8_t getWidth();
    uint8_t getHeight();

    // is there a physical key at this location, if so return its keylayout_t, pass null for current map
    const keylayout_t* getKeyLayout(keyswitch_t k);

    uint8_t getKeyWidth(keyswitch_t k);
    uint8_t getKeyHeight(keyswitch_t k);
    uint8_t getKeyX(keyswitch_t k);
    uint8_t getKeyY(keyswitch_t k);
    inline bool switchIsDown(keyswitch_t k) { return ((_curState[k/_numRows] >> (k%_numRows)) & 0x01); }

    inline bool keyIsDown(keycode_t c) { return switchIsDown(getSwitch(c)); }
    inline bool keyIsUp(keycode_t c) { return !keyIsDown(c); }

    bool keyIsModifier(keycode_t c);

    // did a given key change?
    bool keyChanged(keycode_t c);
    bool keyPressed(keycode_t c);
    bool keyReleased(keycode_t c);

    // returns the number of keys that changed state in the last idle
    keyswitch_t keysPressed();
    keyswitch_t keysReleased();
    keyswitch_t keysChanged();

    bool keyDoubleTapped(keycode_t c);
    bool keyTapped(keycode_t c);

    keycode_t getCode(keyswitch_t k);

    bool isSoftKeyCode(keycode_t c) { return c <= MAX_SOFT_KEY; }

    const keyinfo_t* getKeyInfo(keycode_t c);
    char getKeyChar(keycode_t c);
    const icon_t getKeyIcon(keycode_t c);
    bool getKeyModifier(keycode_t c);
    const char* getKeyLabel(keycode_t c);

    millis_t getHistoryTime(uint8_t n);
    keyswitch_t getHistoryKey(uint8_t n);
    keycode_t getHistoryCode(uint8_t n);
    bool getHistoryPressed(uint8_t n);
    bool getHistoryReleased(uint8_t n) { return !getHistoryPressed(n); }

    void addHistory(keyswitch_t k, keycode_t c, millis_t t, bool d);
    void clearHistory();
    void deleteHistory(uint8_t n);
    void deleteHistory(keycode_t c, bool pressed);

    void printStatus(Stream* c = nullptr);  // dump out the keyboard status, pass null to go to console

    void flush();
  private:

    keyswitch_t getSwitch(keycode_t c);

    bool switchChanged(keyswitch_t k);
    inline bool switchPressed(keyswitch_t k) { return switchChanged(k) && switchIsDown(k); }
    inline bool switchReleased(keyswitch_t k) { return switchChanged(k) && switchIsUp(k); }
    inline bool switchIsUp(keyswitch_t k) { return !switchIsDown(k); }

    millis_t _lastScan = 0;
    millis_t _lastFlush = 0;
    static const millis_t _minScanInterval = 3;
    static const millis_t _maxScanInterval = 5;

    static const millis_t _doubleTappedTime = 500;
    static const millis_t _tappedTime = 500;

    static const uint8_t _numRows = 6;
    static const uint8_t _numColumnsPerMatrix = 7;
    static const uint8_t _numColumns = _numColumnsPerMatrix*2;

    static const uint8_t _leftAddr = 0;
    static const uint8_t _rightAddr = 7;

    MCP23018 _leftMatrix; // columns 0-6
    MCP23018 _rightMatrix; // columns 7-13

    uint8_t _curState[_numColumns];  // assumes less than 8 rows
    uint8_t _lastState[_numColumns];
    uint8_t _changedKeys[_numColumns];

    void scanMatrix();
    void clearKeyChanges();

    const keymap_t* _currentMap;
    const keymap_t* _defaultMap;

    const keyoverlay_t* _currentOverlay;

    const keylayout_t* _currentLayout;
    const keylayout_t* _defaultLayout;

    static const uint8_t _historySize = 10;
    keyevent_t _history[_historySize];

    bool _click = true;
};
#endif