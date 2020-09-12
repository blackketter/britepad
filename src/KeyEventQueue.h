#ifndef _KeyEventQueue_
#define _KeyEventQueue_

#include "Clock.h"
#include "KeyInfo.h"
#include "KeyLayout.h"
#include "KeyEvent.h"
#include "Timer.h"

class KeyEventQueue {
  public:
    KeyEventQueue(int maxEventHistory = 200);

    // override to process events before they get put in the queue
    virtual void processEvent(KeyEvent* e) { };

    keyswitch_t sendKeys();  // send key events to host, returns number of key events sent
    void sendKey(keycode_t code, boolean pressed);

    bool keyTapped(keycode_t c);
    bool keyDoubleTapped(keycode_t c);
    bool keyTapHeld(keycode_t c);

    KeyEvent* getNextEvent();
    KeyEvent* peekNextEvent();
    void addEvent(KeyMatrix* m, keyswitch_t k, keycode_t c, millis_t t, bool d);
    void addEvent(keycode_t c, bool d);
    KeyEvent* history(int i) { KeyEvent* e = _events; while (e && i) { e = e->getPrev(); i--; }; return e; }
    KeyEvent* firstEvent() { KeyEvent* e = _events; while (e) { if (e->getPrev() == nullptr) break; e = e->getPrev(); } return e; }
    KeyEvent* lastEvent(keycode_t c) { KeyEvent* e = _events; while (e) { if (e->code() == c) break; e = e->getPrev();  }; return e; }
    KeyEvent* lastEvent() { return _events; };
    millis_t lastEventTime() { KeyEvent* e = lastEvent();  if (e) return e->time(); else return 0; }
    KeyEvent* prevEvent(KeyEvent* e) { return e->getPrev(); }
    void removeEvent(KeyEvent* e);
    bool keyIsDown(keycode_t c) { KeyEvent* e = lastEvent(c); return (e && e->pressed()); }
    inline bool keyIsUp(keycode_t c) { KeyEvent* e = lastEvent(c); return (!e || !e->pressed()); }

    char getKeyChar(keycode_t c);

    bool isSoftKeyCode(keycode_t c) { return c <= MAX_SOFT_KEY; }

    void repeat();  // only called by timer callback function

    void printStatus(Stream* c = nullptr);  // dump out the keyboard status, pass null to go to console

    void releaseKeys();  // sends release messages to Keyboard for any pressed keys

  private:

    void truncateHistory();

    static const millis_t _doubleTappedTime = 250;
    static const millis_t _tappedTime = 250;

    void clearKeyChanges();

    int _maxEventHistory;
    KeyEvent* _events = nullptr;
    KeyEvent* _lastEvent = nullptr;

    CallbackTimer _repeatTimer;
    static const millis_t _repeatInterval = 50;
    static const millis_t _repeatStart = 500;
    millis_t _lastRepeat = 0;
    int8_t _mouseLeftRightAccel = 0;
    int8_t _mouseUpDownAccel = 0;

    bool _click = true;
};

#endif
