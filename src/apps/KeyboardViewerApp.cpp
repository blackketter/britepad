#include "KeyboardViewerApp.h"

void KeyboardViewerApp::begin() {
  BritepadApp::begin();

  buttons = new KeyboardWidget(_keyMatrix,screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),screen.red,screen.black);

}

void KeyboardViewerApp::run() {

  if (_tutorialMode && !_keyMatrix->getOverlay()) {
    console.debugln("KeyboardViewerApp exiting");
    exit();
  } else {
    draw();
  }
};

void KeyboardViewerApp::end() {
  delete buttons;
  buttons = nullptr;
  _tutorialMode = false;
  console.debugln("KeyboardViewerApp end");
}

void KeyboardViewerApp::draw() {
  buttons->draw();
};

bool KeyboardViewerApp::event(KeyEvent* key) {
  if (getEnabled(KEYBOARD_MODE)) {
    if (!isCurrentApp() && _keyMatrix->getOverlay()) {
        _tutorialMode = true;
        launch();
        console.debugln("Launching KeyboardViewerApp in tutorial mode...");
    }
  }
  return false;
}
