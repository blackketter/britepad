#include "ConsoleApp.h"
#include "Sound.h"
#include <usb_keyboard.h>

ConsoleApp theConsoleApp;

void ConsoleApp::begin(AppMode asMode) {
  BritepadApp::begin(asMode);
  _origPort = console.getPort();

  if (!_terminal) {
    _terminal = new TerminalWidget(_console, screen.clipLeft(),(coord_t)(screen.clipTop()),screen.clipWidth(),(coord_t)(screen.clipHeight()));
  }
  _terminal->clearScreen();
  _terminal->draw();
  console.setPort(_terminal);
  console.printLog();  // begin with the latest logs
}

void ConsoleApp::end() {
  console.setPort(_origPort);
  delete _terminal;
  _terminal = nullptr;
  BritepadApp::end();
}

void ConsoleApp::run() {
  KeyEvent* key = getNextEvent();
  _terminal->key(key);
  _terminal->run();
}
