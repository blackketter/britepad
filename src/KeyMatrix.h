#ifndef _KeyMatrix_
#define _KeyMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "Widgets/icon.h"

typedef uint8_t keyswitch_t;
const keyswitch_t NO_KEY = 0xff;

typedef uint16_t keycode_t;
const keycode_t NO_CODE = 0;

// some new key codes
const keycode_t KEY_LEFT_FN = 200;
const keycode_t KEY_RIGHT_FN = 201;

typedef struct keylayout_t {
  keyswitch_t key;
  uint8_t x;
  uint8_t y;
  uint8_t w;
  uint8_t h;
} keylayout_t;

typedef struct keymap_t {
  keyswitch_t key;
  keycode_t code;
} keymap_t;

typedef struct keyinfo_t {
  keycode_t code;
  char c;
  const char* label;
  const icon_t icon;
} keyinfo_t;

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

    void setLayout(const keylayout_t* l = nullptr);   // pass nullptr to reset to default layout
    const keylayout_t* getLayout() { return _currentLayout; }
    const keylayout_t* getDefaultLayout() { return _defaultLayout; }


    keyswitch_t update();  //returns number of keys changed
    void sendKeys();  // send key events to host

    keyswitch_t numKeys() { return _numColumns * _numRows; }  // returns the total number of keys in the matrix
    uint8_t getWidth();
    uint8_t getHeight();

    // is there a physical key at this location, if so return its keylayout_t, pass null for current map
    const keylayout_t* getKeyLayout(keyswitch_t k);

    uint8_t getKeyWidth(keyswitch_t k);
    uint8_t getKeyHeight(keyswitch_t k);
    uint8_t getKeyX(keyswitch_t k);
    uint8_t getKeyY(keyswitch_t k);

    // returns the number of keys that changed state in the last idle
    keyswitch_t keysPressed();
    keyswitch_t keysReleased();

    // did a given key change?
    inline bool keyChanged(keyswitch_t k) { return (k != NO_KEY) && ((_changedKeys[k/_numRows] >> (k%_numRows)) & 0x01); }
    inline bool keyChanged(keycode_t c) { return (c != NO_CODE) && keyChanged(getSwitch(c)); }
    inline bool keyPressed(keyswitch_t k) { return keyChanged(k) && keyIsDown(k); }
    inline bool keyReleased(keyswitch_t k) { return keyChanged(k) && keyIsUp(k); }
    inline bool keyPressed(keycode_t c) { keyswitch_t k = getSwitch(c); return (c!=NO_CODE) && keyIsDown(k) && keyChanged(k); }
    inline bool keyReleased(keycode_t c) { keyswitch_t k = getSwitch(c); return (c!=NO_CODE) && keyIsUp(k) && keyChanged(k); }

    // marks key as not changed so that the event is not processed
    inline void clearKeyChange(keyswitch_t k) { _changedKeys[k/_numRows] = _changedKeys[k/_numRows] & ~(0x01 << (k%_numRows)); }
    inline void clearKeyChange(keycode_t c) { clearKeyChange(getSwitch(c)); }
    void clearKeyChanges();

    keycode_t getCode(keyswitch_t k);
    keyswitch_t getSwitch(keycode_t c);

    inline bool keyIsDown(keyswitch_t k) { return ((_curState[k/_numRows] >> (k%_numRows)) & 0x01); }
    inline bool keyIsUp(keyswitch_t k) { return !keyIsDown(k); }

    inline bool keyIsDown(keycode_t c) { return keyIsDown(getSwitch(c)); }
    inline bool keyIsUp(keycode_t c) { return !keyIsDown(c); }

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

    void dumpStatus(Stream* c = nullptr);  // dump out the keyboard status, pass null to go to console
  private:
    millis_t _lastScan = 0;
    static const millis_t _minScanInterval = 5;
    static const millis_t _doubleTapTime = 500;

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

    const keymap_t* _currentMap;
    const keymap_t* _defaultMap;

    const keylayout_t* _currentLayout;
    const keylayout_t* _defaultLayout;


    keyswitch_t keysChanged();
    static const uint8_t _historySize = 10;
    keyevent_t _history[_historySize];
};
#endif