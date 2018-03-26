#include "ConsoleApp.h"
#include "Sound.h"
#include <usb_keyboard.h>

ConsoleApp theConsoleApp;

void ConsoleApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);

  if (!_terminal) {
    _terminal = new TerminalWidget(
      screen.clipLeft(),(coord_t)(screen.clipTop()),
      screen.clipWidth(),(coord_t)(screen.clipHeight()),
      100); // scrollback lines
  }
  setPort();
}

void ConsoleApp::end() {
  endPort();
  delete _terminal;
  _terminal = nullptr;
  BritepadApp::end();
}

void ConsoleApp::run() {
  KeyEvent* key = getNextEvent();
  if (key->code(KEY_ESC)){
    exit();
  } else {
    _terminal->key(key);
    _terminal->run();  // redraws as necessary
  }
}

void ConsoleApp::setPort() {
  _origPort = console.getPort();
  console.setPort(_terminal);
  console.printLog();  // begin with the latest logs
}

void ConsoleApp::endPort() {
  console.setPort(_origPort);
}
