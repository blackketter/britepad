#include "FunctionLayerApp.h"
#include "KeyMatrix.h"

const keymap_t functionLayer[] = {
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

FunctionLayerApp theFunctionLayerApp;

void FunctionLayerApp::idle() {
  // switch to the function layer
  if (keyMatrix.keyPressed((keycode_t)KEY_LEFT_FN) ||
      keyMatrix.keyPressed((keycode_t)KEY_RIGHT_FN)) {
    if (keyMatrix.getMap() == functionLayer) {
      keyMatrix.setMap();
    } else {
      keyMatrix.setMap(functionLayer);
    }
  } else if (keyMatrix.keyReleased((keycode_t)KEY_LEFT_FN) ||
             keyMatrix.keyReleased((keycode_t)KEY_RIGHT_FN)) {

    if (keyMatrix.isKeyUp((keycode_t)KEY_RIGHT_FN) && keyMatrix.isKeyUp((keycode_t)KEY_LEFT_FN)) {
      // check for double-tap
      if ((keyMatrix.doubleTapped((keycode_t)KEY_RIGHT_FN) ||
           keyMatrix.doubleTapped((keycode_t)KEY_LEFT_FN))) {
          // double tap!  don't reset the map
        } else {
          keyMatrix.setMap();
        }
    }

  }
};
