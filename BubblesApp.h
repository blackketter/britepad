#ifndef BubblesApp
#define BubblesApp

#include "BPApp.h"

class BubblesApp : BPApp {
  BubblesApp(void) : BPApp() {};
  void init(void);
  void end(void);
  void run(void);
  bool isScreensaver(void) { return true; };
  cosnt char* name(void) { return "Bubbles"; };
}

#endif
