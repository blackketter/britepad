#include "KeyboardViewerApp.h"

KeyboardViewerApp theKeyboardViewerApp;

void KeyboardViewerApp::begin(AppMode asMode) {
  screen.fillScreen(screen.black);
  if (buttons) { console.debugln("WARNING: Beginning KeyboardViewerApp with old buttons!"); }
  buttons = new WidgetGroup();

  keyswitch_t totalKeys = keyMatrix.numKeys();
  coord_t oneKeyWidth = screen.clipWidth()/keyMatrix.getWidth();
  coord_t oneKeyHeight = screen.clipHeight()/keyMatrix.getHeight();

//  alignment_t oldAlign = screen.setTextAlign((alignment_t)(ALIGN_HCENTER+ALIGN_VCENTER));
//  screen.setFont(Arial_12);
//  screen.setTextColor(screen.red);

  for (keyswitch_t k = 0; k < totalKeys; k++) {
    if (keyMatrix.getKey(k, keyMatrix.getDefaultLayout())) {
      coord_t x = screen.clipLeft()+keyMatrix.getKeyX(k)*oneKeyWidth;
      coord_t y = screen.clipTop()+keyMatrix.getKeyY(k)*oneKeyHeight;
      coord_t w = oneKeyWidth*keyMatrix.getKeyWidth(k);
      coord_t h = oneKeyWidth*keyMatrix.getKeyHeight(k);
      Button* b = new Button(x+1, y+1, w-1, h-1, screen.red, false, nullptr, Arial_10_Bold, screen.black, nullptr, (widgetid_t)k);
      buttons->add(b);
    }
  }

  draw();
}

void KeyboardViewerApp::end() {
  delete buttons;
  buttons = nullptr;
}

void KeyboardViewerApp::run() {
  draw();
};

void KeyboardViewerApp::draw() {
  Button* button = (Button*)(buttons->getNext());

  while (button) {
    keyswitch_t k = (keyswitch_t)button->getID();

    keycode_t c = keyMatrix.getCode(k);

    const char* label = keyMatrix.getKeyLabel(c);
    const icon_t icon = keyMatrix.getKeyIcon(c);

    if (icon) {
      label = nullptr;
    }

    button->setHighlighted(keyMatrix.isKeyDown(k));
    button->setTitle(label);
    button->setIcon(icon);

    button = (Button*)(button->getNext());
  }

  buttons->draw();
};