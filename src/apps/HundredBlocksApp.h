#ifndef _HundredBlocksApp_
#define _HundredBlocksApp_

#include "DotCalApp.h"

class HundredBlocksApp : public DotCalApp {
  public:
    void begin(AppMode asMode) override;
    void update() override;
 
    const char* name() { return "100 Blocks"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "100b";
    virtual bool displaysClock() { return false; };

  protected:
    virtual int getDotsWide() override { return 10; }
    virtual int getDotsHigh() override { return 10; }
    virtual uint16_t blocksFilled();

    DotMatrix* dots = nullptr;

  private:
};

#endif


