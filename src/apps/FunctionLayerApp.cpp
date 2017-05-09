#include "KeyMatrix.h"
#include "KeyboardApp.h"

const keymap_t functionLayerMap[] = {
// row 0
    { 0, KEY_F11 },
    { 6, KEY_F1 },
    { 12, KEY_F2 },
    { 18, KEY_F3 },
    { 24, KEY_F4 },
    { 30, KEY_F5 },

    { 72, KEY_F6 },
    { 66, KEY_F7 },
    { 60, KEY_F8 },
    { 54, KEY_F9 },
    { 48, KEY_F10 },
    { 42, KEY_F12 },

// row 1
//    { 0+1, KEY_TAB },
//    { 6+1, KEY_Q },
//    { 12+1, KEY_W },
//    { 18+1, KEY_E },
//    { 24+1, KEY_R },
//    { 30+1, KEY_T },

    { 72+1, KEY_MEDIA_PREV_TRACK },
    { 66+1, KEY_MEDIA_PLAY_PAUSE },
    { 60+1, KEY_MEDIA_NEXT_TRACK },
    { 54+1, KEY_MEDIA_MUTE },
    { 48+1, KEY_MEDIA_VOLUME_DEC },
    { 42+1, KEY_MEDIA_VOLUME_INC },

// row 4
    { 0+4, KEY_LEFT_FN },
    { 42+4, KEY_RIGHT_FN },

  // end of table marker
    { NO_KEY, NO_CODE }
};

class FunctionLayerApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Function\nLayer"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the function layer
        if (keys.keyPressed((keycode_t)KEY_LEFT_FN) || keys.keyPressed((keycode_t)KEY_RIGHT_FN)) {
          if (keys.getMap() != functionLayerMap) {
            keys.setMap(functionLayerMap);
          }
        } else if (
                   (keys.keyReleased((keycode_t)KEY_LEFT_FN) || keys.keyReleased((keycode_t)KEY_RIGHT_FN)) &&  // released fn key
                    keys.keyIsUp((keycode_t)KEY_RIGHT_FN) && keys.keyIsUp((keycode_t)KEY_LEFT_FN) &&           // neither key is still held
                    !keys.doubleTapped((keycode_t)KEY_RIGHT_FN) && !keys.doubleTapped((keycode_t)KEY_LEFT_FN)  // and it's not a double-tap
                  ) {
                keys.setMap();
        }
      }
    };

  private:
};

FunctionLayerApp theFunctionLayerApp;
