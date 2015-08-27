#ifndef _KeyApp_
#define _KeyApp_

#include "BPApp.h"
#include "Britepad.h"

class KeyApp : public BPApp {

  private:
    const char* nameStr;
    const char* keyStr;
    long  specialKey = 0;

  public:
    KeyApp(const char* name, const char* keys) { nameStr = name; keyStr = keys;};
    KeyApp(const char* name, uint8_t key) { nameStr = name; specialKey = key; }

    void begin(void);
    BPApp* run(void);
    const char* name(void) { return nameStr; };
};

#endif
