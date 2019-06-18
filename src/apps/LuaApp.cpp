#ifndef _LuaApp_
#define _LuaApp

#include "ConsoleApp.h"
#include "LuaArduino.h"

class LuaApp : public ConsoleApp {
  public:
    void begin(AppMode asMode);

    void run();

    const char* name() override { return "Lua"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "lua";

  private:
    Lua* _lua = nullptr;
    char repl_buffer[1024];
    uint16_t buff_position = 0;

    virtual void setPort() override {}; // these do nothing now
    virtual void endPort() override {};

    void printPrompt() { if (_terminal) { _terminal->print("\n>> ");} }
};

void LuaApp::begin(AppMode asMode) {
  bool newlua = false;

  ConsoleApp::begin(asMode);

  if (_lua == nullptr) {
    _lua = new Lua();
    newlua = true;
  }

  _lua->setStream(_terminal);

  if (newlua) {
    _lua->help();
    printPrompt();
  }
}

void LuaApp::run() {

  if (_terminal->available() > 0) {
    char new_char = _terminal->read();
    if ((new_char >= 32 && new_char <= 127) // printable character
        || new_char == '\n' // newline
        || new_char == '\r' // return
        || new_char == 4    // EOT end of transmission
        || new_char == 8    // backspace
        || new_char == 9    // tab
        ) {
      repl_buffer[buff_position] = new_char;
      // echo new char
      _terminal->write(new_char);
      buff_position++;
    }
    // TODO handle escape sequences 27 (and arrow keys)
  }

  // if no characters received skip the rest of the loop
  if (buff_position != 0) {
    // if backspace was pressed
    if (repl_buffer[buff_position-1] == 8) {
      if (buff_position == 1)
        // just remove the backspace character
        buff_position--;
      else
        // remove both the backspace character and the previously entered character
        buff_position = buff_position - 2;
    }
    // send line on Ctrl-D or return
    else if (repl_buffer[buff_position-1] == '\r' ||
             repl_buffer[buff_position-1] == 4) {
      // set the last character to the null char (should overwrite the EOT)
      repl_buffer[buff_position-1] = '\0';

      if (_lua) {
        _lua->loadString(repl_buffer);
      } else {
        _terminal->println("No Lua Interpreter!");
      }
      // reset buffer index
      buff_position = 0;

      printPrompt();
    }
  }

  ConsoleApp::run();
}

LuaApp theLuaApp;

#endif
