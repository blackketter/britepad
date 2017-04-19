#include "FunctionLayerApp.h"
#include "KeyboardMatrix.h"

const keylayout_t functionLayer[] = {
// row 0
    { 0, KEY_F11, 0, 0 },
    { 6, KEY_F1, 1, 0 },
    { 12, KEY_F2, 2, 0 },
    { 18, KEY_F3, 3, 0 },
    { 24, KEY_F4, 4, 0 },
    { 30, KEY_F5, 5, 0 },

    { 72, KEY_F6, 6+6, 0 },
    { 66, KEY_F7, 7+6, 0 },
    { 60, KEY_F8, 8+6, 0 },
    { 54, KEY_F9, 9+6, 0 },
    { 48, KEY_F10, 10+6, 0 },
    { 42, KEY_F12, 11+6, 0 },

// row 1
    { 0+1, KEY_MEDIA_VOLUME_DEC, 0, 1 },
//    { 6+1, KEY_Q, 1, 1 },
//    { 12+1, KEY_W, 2, 1 },
//    { 18+1, KEY_E, 3, 1 },
//    { 24+1, KEY_R, 4, 1 },
//    { 30+1, KEY_T, 5, 1 },

//    { 72+1, KEY_Y, 6+6, 1 },
    { 66+1, KEY_MEDIA_PREV_TRACK, 7+6, 1 },
    { 60+1, KEY_MEDIA_PLAY_PAUSE, 8+6, 1 },
    { 54+1, KEY_MEDIA_NEXT_TRACK, 9+6, 1 },
    { 48+1, KEY_MEDIA_MUTE, 10+6, 1 },
    { 42+1, KEY_MEDIA_VOLUME_INC, 11+6, 1 },

// row 4
    { 0+4, KEY_LEFT_FN, 0, 4 },
    { 42+4, KEY_RIGHT_FN, 11+6, 4 },

  // end of table marker
    { NO_KEY, 0, 0, 0 }
};

FunctionLayerApp theFunctionLayerApp;

void FunctionLayerApp::idle() {
  // switch to the function layer
  if (keyboardMatrix.keyPressed((keycode_t)KEY_LEFT_FN) ||
      keyboardMatrix.keyPressed((keycode_t)KEY_RIGHT_FN)) {

    keyboardMatrix.setLayout(functionLayer);

  } else if (keyboardMatrix.keyReleased((keycode_t)KEY_LEFT_FN) ||
             keyboardMatrix.keyReleased((keycode_t)KEY_RIGHT_FN)) {

    keyboardMatrix.setLayout();

  }
};
