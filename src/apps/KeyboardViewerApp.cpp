#include "KeyboardViewerApp.h"

KeyboardViewerApp theKeyboardViewerApp(&keys);

void KeyboardViewerApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  buttons = new KeyboardWidget(_keyMatrix,screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),screen.red,screen.black);

}

void KeyboardViewerApp::end() {
  delete buttons;
  buttons = nullptr;
}

void KeyboardViewerApp::run() {

  if (tutorialMode && !_keyMatrix->getOverlay()) {
    tutorialMode = false;
    if (lastApp) {
      launchApp(lastApp, lastMode);
    }
  } else {
    draw();
  }
};

void KeyboardViewerApp::draw() {
  buttons->draw();
};

bool KeyboardViewerApp::event(KeyEvent* key) {
  if (getEnabled(KEYBOARD_MODE)) {
//    console.debugln("keyboardviewerapp idle");
    if (!isCurrentApp() && _keyMatrix->getOverlay()) {
        BritepadApp* currApp = britepad.currentApp();
        if (currApp && currApp != this) {
          lastApp = currApp;
          lastMode = lastApp->getAppMode();
        }
        tutorialMode = true;
        launch();
        console.debugln("Launching KeyboardViewerApp in tutorial mode...");
    }
  }
  return false;
}