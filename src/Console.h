#ifndef _Console_
#define _Console_

#include <Arduino.h>

class Console : public Stream {
  public:
    void begin();
	  void debugf(const char* format, ...);
	  void debugln(const char* s);
    void debug(bool enable) { _debugEnabled = enable; };

// low level
	  int available();
	  int read();
    int peek();
    void flush();
    size_t write(uint8_t b);

// todos:
    // void enable(bool enabled);

  private:
    void debugPrefix();
    bool _debugEnabled = true;
};

extern Console console;

#endif
