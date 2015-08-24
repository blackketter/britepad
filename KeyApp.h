#ifndef _KeyApp_
#define _KeyApp_

#include "BPApp.h"
#include "Britepad.h"

class KeyApp : public BPApp {

  private:
    const char* nameStr;
    const char* keyStr;

  public:
    KeyApp(const char* name, const char* keys) { nameStr = name; keyStr = keys;};
    void begin(void);
    BPApp* run(void);
    bool isScreensaver(void) { return true; };
    const char* name(void) { return nameStr; };
};

#endif
