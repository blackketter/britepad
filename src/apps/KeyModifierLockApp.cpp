#include "KeyMatrix.h"
#include "KeyboardApp.h"

void unlockTimerCallback(void* app);

const keycode_t modifierKeys[] = {
  MODIFIERKEY_LEFT_SHIFT,
  MODIFIERKEY_LEFT_GUI,
  MODIFIERKEY_LEFT_ALT,
  MODIFIERKEY_LEFT_CTRL,
  KEY_LEFT_FN,
  MODIFIERKEY_RIGHT_SHIFT,
  MODIFIERKEY_RIGHT_GUI,
  MODIFIERKEY_RIGHT_ALT,
  MODIFIERKEY_RIGHT_CTRL,
  KEY_RIGHT_FN,
  NO_CODE
};

class KeyModifierLockApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "modl";
    const char* name() { return "Modifier Lock"; };
    EventPriority eventPriority() { return PRIORITY_FIRST; }
    bool event(KeyEvent* key) {

      if (getEnabled(KEYBOARD_MODE)) {

        // on each key event, restart the unlock timer
        unlockTimer.setMillis(lockTimeout, unlockTimerCallback, (void*)this);

        if (!key->isModifier()) {
          if (key->released()) {
            if (!key->isMouseKey()) {
              unlockAll();
            } else {
              mouseKeyHit = true;
            }
          } else {
            if (anyLocked() && key->pressed(KEY_ESC)) {
              flipLockStates(doubleLocked, toUnlock);
              key->clear();
              unlockAll();
            }
          }
        } else {
          lockState state = getLockState(key->code());

          //console.debugf("modifier: code:%d pressed:%d modifier:%d state: %d\n", key->code(),key->pressed(),key->isModifier(), state);

          if (key->pressed()) {
            unlockTimer.cancel();
            if (state == unlocked) {
              setLockState(key->code(), toLock);
            }
          } else {
            if (state == toLock) {
              if (mouseKeyHit) {
                unlockAll();
                mouseKeyHit = false;
              } else if (keyEvents.keyTapped(key->code())) {
                setLockState(key->code(), locked);
                key->clear();
              } else {
                setLockState(key->code(), unlocked);
              }
            } else if (state == locked) {
//              setLockState(key->code(), unlocked);
              setLockState(key->code(), doubleLocked);
              key->clear();
            } else if (state == doubleLocked) {
              flipLockStates(doubleLocked, toUnlock);
              flipLockStates(locked, toUnlock);
              flipLockStates(toLock, unlocked);
            }
          }
        }

        //console.debugln("reset unlock timeout");
        sendUpdates();
      }
      return false;
    };

    void unlock() {
      unlockAll();
      sendUpdates();
    }

  private:
    enum lockState {
      unlocked,
      locked,
      toUnlock,
      toLock,
      doubleLocked,
    };

    void unlockAll() {
      //console.debugln("unlock all!");hhh
      flipLockStates(locked, toUnlock);
      flipLockStates(toLock, unlocked);
    }

    void sendUpdates() {
        for (int i = 0; i < modifierCount; i++) {
          if (state[i] == toUnlock) {
            state[i] = unlocked;
            console.debugf("unlocking %d\n",modifierKeys[i]);
            keyEvents.addEvent(nullptr, NO_KEY, modifierKeys[i], Uptime::millis(), false);
          }
        }

        float pitch = Sound::MIDDLE_C_FREQ;
        for (int i = 0; i < modifierCount; i++) {
          if (getModifierState(i)) {
            pitch = pitch * (3.0/2.);  // the more modifier keys held, we raise the frequency by a perfect fifth
          }
        }
        if (pitch == Sound::MIDDLE_C_FREQ) {
          sound.tone(pitch, 0.0);
        } else {
          sound.tone(pitch,0.5);
        }
    }

    void flipLockStates(lockState from, lockState to) {
      for (int j = 0; j < modifierCount; j++) {
        if (state[j] == from) {
          state[j] = to;
        }
      }
    }

    lockState getLockState(keycode_t c) {
      lockState l = state[getModifierIndex(c)];
      //console.debugf("getting %d lock state of %d\n",c, l);
      return l;
    };

    void setLockState(keycode_t c, lockState l) {
      //console.debugf("setting %d lock state to %d\n",c, l);
      state[getModifierIndex(c)] = l;
    };

    bool getModifierState(int i) {
     return state[i] == locked || state[i] == toLock || state[i] == doubleLocked;
    }

    int getModifierIndex(keycode_t c) {
      for (int j = 0; j < modifierCount; j++) {
        if (c == modifierKeys[j]) {
          return j;
        }
      }
      return -1;
    }

    bool anyLocked() {
      for (int i = 0; i < modifierCount; i++) {
        if (getModifierState(i)) {
          return true;
        }
      }
      return false;
    }


    static const int modifierCount = sizeof(modifierKeys)/sizeof(keycode_t);
    lockState state[modifierCount];
    CallbackTimer unlockTimer;
    static const millis_t lockTimeout = 1000;
    bool mouseKeyHit = false;
};

void unlockTimerCallback(void* app) {
  //console.debugln("unlock timer callback");
  ((KeyModifierLockApp*)app)->unlock();
}

KeyModifierLockApp theKeyModifierLockApp;
