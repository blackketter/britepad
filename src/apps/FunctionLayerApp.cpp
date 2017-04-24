#include "FunctionLayerApp.h"
#include "KeyMatrix.h"

const keylayout_t functionLayer[] = {
// row 0
    { 0, KEY_F11, 0, 0, 1, 1 },
    { 6, KEY_F1, 1, 0, 1, 1 },
    { 12, KEY_F2, 2, 0, 1, 1 },
    { 18, KEY_F3, 3, 0, 1, 1 },
    { 24, KEY_F4, 4, 0, 1, 1 },
    { 30, KEY_F5, 5, 0, 1, 1 },

    { 72, KEY_F6, 6+6, 0, 1, 1 },
    { 66, KEY_F7, 7+6, 0, 1, 1 },
    { 60, KEY_F8, 8+6, 0, 1, 1 },
    { 54, KEY_F9, 9+6, 0, 1, 1 },
    { 48, KEY_F10, 10+6, 0, 1, 1 },
    { 42, KEY_F12, 11+6, 0, 1, 1 },

// row 1
//    { 0+1, KEY_TAB, 0, 1, 1, 1 },
//    { 6+1, KEY_Q, 1, 1, 1, 1 },
//    { 12+1, KEY_W, 2, 1, 1, 1 },
//    { 18+1, KEY_E, 3, 1, 1, 1 },
//    { 24+1, KEY_R, 4, 1, 1, 1 },
//    { 30+1, KEY_T, 5, 1, 1, 1 },

    { 72+1, KEY_MEDIA_PREV_TRACK, 6+6, 1, 1, 1 },
    { 66+1, KEY_MEDIA_PLAY_PAUSE, 7+6, 1, 1, 1 },
    { 60+1, KEY_MEDIA_NEXT_TRACK, 8+6, 1, 1, 1 },
    { 54+1, KEY_MEDIA_MUTE, 9+6, 1, 1, 1 },
    { 48+1, KEY_MEDIA_VOLUME_DEC, 10+6, 1, 1, 1 },
    { 42+1, KEY_MEDIA_VOLUME_INC, 11+6, 1, 1, 1 },

// row 4
    { 0+4, KEY_LEFT_FN, 0, 4, 1, 1 },
    { 42+4, KEY_RIGHT_FN, 11+6, 4, 1, 1 },

  // end of table marker
    { NO_KEY, 0, 0, 0, 1, 1 }
};

FunctionLayerApp theFunctionLayerApp;

void FunctionLayerApp::idle() {
  // switch to the function layer
  if (keyMatrix.keyPressed((keycode_t)KEY_LEFT_FN) ||
      keyMatrix.keyPressed((keycode_t)KEY_RIGHT_FN)) {

    keyMatrix.setLayout(functionLayer);

  } else if (keyMatrix.keyReleased((keycode_t)KEY_LEFT_FN) ||
             keyMatrix.keyReleased((keycode_t)KEY_RIGHT_FN)) {
    if (keyMatrix.isKeyUp((keycode_t)KEY_RIGHT_FN) && keyMatrix.isKeyUp((keycode_t)KEY_LEFT_FN)) {
      keyMatrix.setLayout();
    }
  }
};
