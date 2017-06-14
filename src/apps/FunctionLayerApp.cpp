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
    { MODIFIERKEY_RIGHT_SHIFT, MODIFIERKEY_RIGHT_SHIFT },
    { MODIFIERKEY_LEFT_SHIFT, MODIFIERKEY_LEFT_SHIFT },

    { KEY_J, KEY_MOUSE_MOVE_UP },
    { KEY_K, KEY_MOUSE_MOVE_DOWN },
    { KEY_H, KEY_MOUSE_MOVE_LEFT },
    { KEY_L, KEY_MOUSE_MOVE_RIGHT },
    { KEY_ENTER, KEY_MOUSE_BUTTON_LEFT },
    { MODIFIERKEY_RIGHT_GUI, KEY_MOUSE_BUTTON_MIDDLE },
    { KEY_SPACE, KEY_MOUSE_BUTTON_RIGHT },
    { KEY_LEFT_BRACE, KEY_MOUSE_BUTTON_BACK },
    { KEY_RIGHT_BRACE, KEY_MOUSE_BUTTON_FORWARD },
    { KEY_UP, KEY_MOUSE_SCROLL_UP },
    { KEY_DOWN, KEY_MOUSE_SCROLL_DOWN },
    { KEY_LEFT, KEY_MOUSE_SCROLL_LEFT },
    { KEY_RIGHT, KEY_MOUSE_SCROLL_RIGHT },
    { MODIFIERKEY_LEFT_GUI, MODIFIERKEY_MOUSE },
    { MODIFIERKEY_LEFT_ALT, MODIFIERKEY_MOUSE_SCROLL },

    // end of table marker
    { NO_CODE, NO_CODE }  // replace the second value with ANY_CODE to make overlay transparent
};

class FunctionLayerApp : public KeyboardApp {

  public:

    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Function Layer"; };

    void event(KeyEvent* key) {
      if (getEnabled(KEYBOARD_MODE)) {
        // switch to the function layer
        if (key->pressed(KEY_LEFT_FN)) {
          if (keys.getOverlay() != functionOverlay) {
            keys.setOverlay(functionOverlay);
            // jiggle the mouse to make cursor show up
            Mouse.move(-1, 0);
            Mouse.move(1, 0);
          }
        } else if (
                   (key->released(KEY_LEFT_FN))  // released fn key
                  ) {
                keys.setOverlay();
        }
      }
    };

  private:
};

FunctionLayerApp theFunctionLayerApp;
