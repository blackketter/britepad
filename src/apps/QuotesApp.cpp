#include "BritepadShared.h"
#include "ObliqueStrategies.h"
#include "ScreensaverApp.h"

class QuotesApp : public ScreensaverApp {
  public:
    void run() override {
      if (lastDrawn == 0) {
        clearScreen();
        screen.setFont(&Arial_20_Bold);
        screen.setTextColor(screen.white, bgColor());
        screen.setCursor(screen.clipMidWidth(), screen.clipMidHeight());
        screen.setTextAlign((alignment_t)(ALIGN_VCENTER|ALIGN_HCENTER));

        String wrapped;
        const char* strat = randomObliqueStrategy();
        console.debugf("Quote: %s\n",strat);
        screen.softWrapText(wrapped, strat);
        screen.drawString(wrapped);
        screen.setTextAlign();

        lastDrawn = Uptime::millis();
      }

    }
    void begin(AppMode asMode) override { lastDrawn = 0; }

    const char* name() override { return "Quotes"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "quot";

  protected:
    millis_t lastDrawn = 0;
};

QuotesApp theQuotesApp;


