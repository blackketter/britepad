#ifndef _FunctionLayerApp_
#define _FunctionLayerApp_

#include "KeyboardApp.h"

class FunctionLayerApp : public KeyboardApp {

  public:
    appid_t id() { return ID; };
    static constexpr appid_t ID = "lbar";
    const char* name() { return "Function\nLayer"; };

    void idle();
  private:
};

#endif // _FunctionLayerApp_