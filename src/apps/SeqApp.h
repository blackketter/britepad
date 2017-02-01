#ifndef _SeqApp_
#define _SeqApp_

#include "DotsDisplayApp.h"

class SeqApp : public DotsDisplayApp {
  public:
    void run();

    const char* name() { return "Sequencer"; };
    static constexpr appid_t ID = "seqr";
    appid_t id() { return ID; };

    bool disablesScreensavers() { return true; }
    bool canBeScreensaver() { return false; }
    bool canBeMouse() { return false; }
    bool canBeInteractive() { return true; }
    AppType getAppType() { return INTERACTIVE_APP; }

  protected:
    virtual int getDotsWide() { return 12; }
    virtual int getDotsHigh() { return 9; }
    millis_t lastRun = 0;
    int bpm = 138;
    static const int maxBpm = 400;
    static const int minBpm = 40;

    int currCol = 0;
    bool tick = true;
};

#endif



