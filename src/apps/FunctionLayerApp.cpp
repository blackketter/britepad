#include "KeyMatrix.h"
#include "KeyboardApp.h"

const keyoverlay_t functionOverlay[] = {
// row 0
    { KEY_EQUAL, KEY_F11 },
    { KEY_1, KEY_F1 },
    { KEY_2, KEY_F2 },
    { KEY_3, KEY_F3 },
    { KEY_4, KEY_F4 },
    { KEY_5, KEY_F5 },

    { KEY_6, KEY_F6 },
    { KEY_7, KEY_F7 },
    { KEY_8, KEY_F8 },
    { KEY_9, KEY_F9 },
    { KEY_0, KEY_F10 },
    { KEY_MINUS, KEY_F12 },

    { KEY_Y, KEY_MEDIA_PREV_TRACK },
    { KEY_U, KEY_MEDIA_PLAY_PAUSE },
    { KEY_I, KEY_MEDIA_NEXT_TRACK },
    { KEY_O, KEY_MEDIA_MUTE },
    { KEY_P, KEY_MEDIA_VOLUME_DEC },
    { KEY_BACKSLASH, KEY_MEDIA_VOLUME_INC },

    { KEY_LEFT_FN, KEY_LEFT_FN },
    { KEY_RIGHT_FN, KEY_RIGHT_FN },

    // end of table marker
    { NO_CODE, NO_CODE }  // replace the second value with ANY_CODE to make overlay transparent
};

class FunctionLayerApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Function Layer"; };

    void idle() {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the function layer
        if (keys.keyPressed(KEY_LEFT_FN) || keys.keyPressed(KEY_RIGHT_FN)) {
          if (keys.getOverlay() != functionOverlay) {
            keys.setOverlay(functionOverlay);
          }
        } else if (
                   (keys.keyReleased(KEY_LEFT_FN) || keys.keyReleased(KEY_RIGHT_FN))  // released fn key
//                    && keys.keyIsUp(KEY_RIGHT_FN) && keys.keyIsUp(KEY_LEFT_FN)           // neither key is still held
//                    && !keys.keyDoubleTapped(KEY_RIGHT_FN) && !keys.keyDoubleTapped(KEY_LEFT_FN)  // and it's not a double-tap
                  ) {
                keys.setOverlay();
        }
      }
    };

  private:
};

FunctionLayerApp theFunctionLayerApp;
