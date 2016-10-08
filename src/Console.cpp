#include "Console.h"
#include <stdarg.h>
#include <stdio.h>

Console console;

void Console::begin() {
  Serial.begin(115200);
};


int Console::available() {
  return Serial.available();
};


int Console::read() {
  return Serial.read();
};


int Console::peek() {
  return Serial.peek();
};


void Console::flush() {
  Serial.flush();
};


size_t Console::write(uint8_t b) {
  return Serial.write(b);
};

// todo: can we override this properly?
void Console::debugf(const char* format, ...) {
  if (!_debugEnabled) return;

  char foo[500];
  debugPrefix();
  va_list argptr;
  va_start(argptr, format);
  vsprintf(foo, format, argptr);
  va_end(argptr);
  Serial.print(foo);
};

void Console::debugln(const char* s) {
  if (!_debugEnabled) return;
  debugPrefix();
  Serial.println(s);
}

void Console::debugPrefix() {
  Serial.print(::millis());
  Serial.print(": ");
}
