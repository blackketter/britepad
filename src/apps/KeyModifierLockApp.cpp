#include "KeyMatrix.h"
#include "KeyboardApp.h"

class ModifierLockApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "modl";
    const char* name() { return "Modifier Lock"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {

        int c = keys.keysChanged();
        if (c) { lastKey = Uptime::millis(); }

        // go through the keys oldest to newest
        for (int i = c-1; i >= 0; i--) {
          keycode_t code = keys.getHistoryCode(i);
          bool pressed = keys.getHistoryPressed(i);
          bool released = !pressed;
          bool modifier = keys.keyIsModifier(code);
          lockState state = getLockState(code);

          console.debugf("modifier[%d]: code:%d pressed:%d modifier:%d\n", i,code,pressed,modifier);

          if (!modifier && pressed) {
            flipLockStates(locked,toUnlock);
            flipLockStates(toLock,unlocked);
          }

          if (modifier && pressed) {
            if (state == unlocked) {
              setLockState(code, toLock);
              setModifierState(code, true);
            }
          }

          if (modifier && released) {
            if (state == toLock) {
              setLockState(code, locked);
              keys.deleteHistory(code, false);
            } else if (state == locked) {
              setLockState(code, unlocked);
            }
            setModifierState(code, false);
          }
        }

        // time out locks
        if (c) {
          lastKey = Uptime::millis();
          console.debugln("reset unlock timeout");
        } else if (lastKey && ((Uptime::millis() - lastKey) > lockTimeout)){
          flipLockStates(locked, toUnlock);
          flipLockStates(toLock, unlocked);
          lastKey = 0;
          console.debugln("unlock timeout");
        }

        for (int i = 0; i < modifierCount; i++) {
          if (state[i] == toUnlock) {
            console.debugf("unlocking %d\n",modifierKeys[i]);
            keys.addHistory(NO_KEY, modifierKeys[i], Uptime::millis(), false);
            state[i] = unlocked;
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
    };

  private:
    enum lockState {
      unlocked,
      locked,
      toUnlock,
      toLock,
    };

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
      console.debugf("setting %d lock state to %d\n",c, l);
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
    millis_t lastKey;
    static const millis_t lockTimeout = 2000;
};

ModifierLockApp theModifierLockApp;
