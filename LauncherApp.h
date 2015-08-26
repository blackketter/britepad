#ifndef _LAUNCHERAPP_
#define _LAUNCHERAPP_

#include "BPApp.h"
#include "Britepad.h"

class LauncherApp : public BPApp {
  private:
    const static int hbuttons = 4;
    const static int vbuttons = 3;
    const static int noButton = -1;
    BPApp* apps[hbuttons*vbuttons];
    int highlightedButton = noButton;

    int buttonHit(int x, int y);
    void drawButton(int i, bool highlighted = false);

  public:
    LauncherApp(void);
    void begin(void);
    BPApp* run(void);
    void setButton(int i, BPApp* b);
    int totalButtons(void) { return hbuttons * vbuttons; };
    BPApp* getButton(int i);
    const char* name(void) { return "Launcher"; };
};
#endif

