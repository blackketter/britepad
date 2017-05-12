#include "KeyboardViewerApp.h"

KeyboardViewerApp theKeyboardViewerApp;

void KeyboardViewerApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  if (buttons) { console.debugln("WARNING: Beginning KeyboardViewerApp with old buttons!"); }
  buttons = new WidgetGroup();

  keyswitch_t totalKeys = keys.numKeys();
  coord_t oneKeyWidth = screen.clipWidth()/keys.getWidth();
  coord_t oneKeyHeight = screen.clipHeight()/keys.getHeight();

  for (keyswitch_t k = 0; k < totalKeys; k++) {
    if (keys.getKeyLayout(k)) {
      coord_t x = screen.clipLeft()+keys.getKeyX(k)*oneKeyWidth;
      coord_t y = screen.clipTop()+keys.getKeyY(k)*oneKeyHeight;
      coord_t w = oneKeyWidth*keys.getKeyWidth(k);
      coord_t h = oneKeyWidth*keys.getKeyHeight(k);
      Button* b = new Button(x+1, y+1, w-1, h-1, screen.red, false, "", Arial_10_Bold, screen.black, nullptr, (widgetid_t)k);
      buttons->add(b);
    }
  }
}

void KeyboardViewerApp::end() {
  delete buttons;
  buttons = nullptr;
}

void KeyboardViewerApp::run() {

  if (tutorialMode && (keys.getMap() == keys.getDefaultMap())) {
    if (lastApp) {
      launchApp(lastApp, lastMode);
    }
    tutorialMode = false;
  }

  draw();
};

void KeyboardViewerApp::draw() {
  Button* button = (Button*)(buttons->getNext());

  while (button) {
    keyswitch_t k = (keyswitch_t)button->getID();

    keycode_t c = keys.getCode(k);

    const char* title = keys.getKeyLabel(c);
    const icon_t icon = keys.getKeyIcon(c);
    bool down = keys.switchIsDown(k);

    if (icon) {
      title = nullptr;
    }

    bool changed = false;
    if (button->getTitle() != title) changed = true;
    if (button->getIcon().getData() != icon) changed = true;
    if (button->getHighlighted() != down) changed = true;

    if (changed) {
      button->setHighlighted(keys.switchIsDown(k));
      button->setTitle(title);
      button->setIcon(icon);
      button->draw();
    }
    button = (Button*)(button->getNext());
  }
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