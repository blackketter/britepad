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
