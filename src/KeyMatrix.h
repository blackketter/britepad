#ifndef _KeyMatrix_
#define _KeyMatrix_

#include "Clock.h"
#include "MCP23018.h"
#include "Widgets/icon.h"
#include "KeyInfo.h"
#include "KeyLayout.h"
#include "KeyEvent.h"
#include "Timer.h"

class KeyMatrix {
  public:
    virtual void begin();
    virtual void update();
    virtual const char* name() = 0;

    void setMap(const keymap_t* m = nullptr);  // pass nullptr to reset to default map
    const keymap_t* getMap() { return _currentMap; }
    const keymap_t* getDefaultMap() { return _defaultMap; }

    const void setOverlay(const keyoverlay_t* o = nullptr) { _currentOverlay = o; }
    const keyoverlay_t* getOverlay() { return _currentOverlay; }
    keycode_t lookupOverlay(keycode_t c);

    void setLayout(const keylayout_t* l = nullptr);   // pass nullptr to reset to default layout
    const keylayout_t* getLayout() { return _currentLayout; }
    const keylayout_t* getDefaultLayout() { return _defaultLayout; }

    uint8_t getWidth();
    uint8_t getHeight();
    keyswitch_t getNthKey(int i) { if (_currentLayout) { return _currentLayout[i].key;} else { return NO_KEY;}; }
    // is there a physical key at this location, if so return its keylayout_t, pass null for current map
    const keylayout_t* getKeyLayout(keyswitch_t k);

    uint8_t getKeyWidth(keyswitch_t k);
    uint8_t getKeyHeight(keyswitch_t k);
    uint8_t getKeyX(keyswitch_t k);
    uint8_t getKeyY(keyswitch_t k);
    keycode_t getCode(keyswitch_t k);

    const icon_t getKeyIcon(keycode_t c);
    modifierkey_t getKeyModifier(keycode_t c);
    const char* getKeyLabel(keycode_t c);

    void repeat();  // only called by timer callback function

    const keyinfo_t* getKeyInfo(keycode_t c);

  protected:
    keyswitch_t getSwitch(keycode_t c);
    int getKeyInfoIndex(keycode_t c);

    millis_t _nextScan = 0;
    static const millis_t _minScanInterval = 3;
    static const millis_t _debounceInterval = 20; // was 5ms, but that was too short, some switches were bouncing

    millis_t _lastFlush = 0;

    const keymap_t* _currentMap;
    const keymap_t* _defaultMap;

    const keyoverlay_t* _currentOverlay;

    const keylayout_t* _currentLayout;
    const keylayout_t* _defaultLayout;
};
#endif