#include "KeyboardViewerApp.h"

KeyboardViewerApp theKeyboardViewerApp;

void KeyboardViewerApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  buttons = new KeyboardWidget(&keys,screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),screen.red,screen.black);

}

void KeyboardViewerApp::end() {
  delete buttons;
  buttons = nullptr;
}

void KeyboardViewerApp::run() {

  if (tutorialMode && (keys.getMap() == keys.getDefaultMap())) {
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

void KeyboardViewerApp::idle() {
  if (getEnabled(KEYBOARD_MODE)) {
    if (!isCurrentApp() && (keys.getMap() != keys.getDefaultMap())) {
        BritepadApp* currApp = britepad.currentApp();
        if (currApp && currApp != this) {
          lastApp = currApp;
          lastMode = lastApp->getAppMode();
        }
        tutorialMode = true;
        launch();
  //      console.debugln("Launching KeyboardViewerApp in tutorial mode...");
    }
  }
}