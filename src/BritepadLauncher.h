#ifndef _Britepad_
#define _Britepad_
#include "BritepadShared.h"
#include "apps/BritepadApp.h"
#include "Launcher.h"

class BritepadLauncher : public Launcher {
  public:
    void begin() override;
    void loop() override;
    void idle() override;

    void drawBars(bool update = false); // draw the status and info bars
    void updateStatusBar();

    static const int minBrightness = 20;
    void wakeHost() override;

  protected:
    bool setApp(App* newApp, AppMode asMode) override;  // sets the current app

  private:
    void drawInfoBar(bool update = false);
    void drawStatusBar(bool update);

    BritepadApp* currentBritepadApp();
    void resetClipRect();

    static const millis_t ambientUpdateInterval = 100;
    static const millis_t showClockDur = 2000;
    static const coord_t _statusBarHeight = 16;
    CallbackTimer     statusBarUpdateTimer;
    CallbackTimer     backlightTimer;
};

#endif
