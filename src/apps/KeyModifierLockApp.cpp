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

    void idle(KeyEvent* key) {

      if (getEnabled(KEYBOARD_MODE)) {

       // on each key press event, reset the unlock timer
       if (key->pressed()) {
         unlockTimer.setMillis(lockTimeout, unlockTimerCallback, (void*)this);
       }

       lockState state = getLockState(key->code());

        if (key->isModifier()) {
//          console.debugf("modifier: code:%d pressed:%d modifier:%d\n", key->code(),key->pressed(),key->isModifier());
        }

        if (!key->isModifier() && key->pressed()) {
          unlockAll();
        }

        if (key->isModifier() && key->pressed()) {
          if (state == unlocked) {
            setLockState(key->code(), toLock);
            setModifierState(key->code(), true);
          }
        }

        if (key->isModifier() && key->released()) {
          if (state == toLock) {
            setLockState(key->code(), locked);
            key->clear();
          } else if (state == locked) {
            setLockState(key->code(), unlocked);
          }
          setModifierState(key->code(), false);
        }

//        console.debugln("reset unlock timeout");
        sendUpdates();
      }
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
    };

    void unlockAll() {
          flipLockStates(locked, toUnlock);
          flipLockStates(toLock, unlocked);
    }

    void sendUpdates() {
        for (int i = 0; i < modifierCount; i++) {
          if (state[i] == toUnlock) {
            state[i] = unlocked;
//            console.debugf("unlocking %d\n",modifierKeys[i]);
            keys.addEvent(NO_KEY, modifierKeys[i], Uptime::millis(), false);
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
      return state[getModifierIndex(c)];
    };

    void setLockState(keycode_t c, lockState l) {
      //console.debugf("setting %d lock state to %d\n",c, l);
      state[getModifierIndex(c)] = l;
    };

    void setModifierState(keycode_t c, bool s) {
      modifierState[getModifierIndex(c)] = s;
    };

    bool getModifierState(int i) {
     return state[i] == locked || state[i] == toLock || modifierState[i];
    }

    int getModifierIndex(keycode_t c) {
      for (int j = 0; j < modifierCount; j++) {
        if (c == modifierKeys[j]) {
          return j;
        }
      }
      return -1;
    }

    static const int modifierCount = sizeof(modifierKeys)/sizeof(keycode_t);
    lockState state[modifierCount];
    bool modifierState[modifierCount];
    Timer unlockTimer;
    static const millis_t lockTimeout = 2000;
};

void unlockTimerCallback(void* app) {
  ((KeyModifierLockApp*)app)->unlock();
}

KeyModifierLockApp theKeyModifierLockApp;
