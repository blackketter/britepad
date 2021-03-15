#include "BritepadShared.h"
#include "ObliqueStrategies.h"
#include "JennyHolzer.h"
#include "CognitiveBiases.h"
#include "ScreensaverApp.h"

class QuotesApp : public ScreensaverApp {
  public:
    void run() override {
      if (lastDrawn == 0) {
        clearScreen();
        screen.setFont(font());
        screen.setTextColor(screen.white, bgColor());
        screen.setCursor(screen.clipMidWidth(), screen.clipMidHeight());
        screen.setTextAlign((alignment_t)(ALIGN_VCENTER|ALIGN_HCENTER));

        String wrapped;
        const char* strat = randomQuote();
        console.debugf("Quote: %s\n",strat);
        screen.softWrapText(wrapped, strat);
        screen.drawString(wrapped);
        screen.setTextAlign();

        lastDrawn = Uptime::millis();
      }

    }
    void begin() override { lastDrawn = 0; }
    virtual const ILI9341_t3_font_t* font() { return &Arial_20_Bold; }
  protected:
    millis_t lastDrawn = 0;
    virtual const char* randomQuote() = 0;

};

class HolzerApp : public QuotesApp {

    const char* name() override { return "Holzer"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "holz";

  protected:
    const char* randomQuote() override { return randomJennyHolzer(); }
};
HolzerApp theHolzerApp;

class ObliqueApp : public QuotesApp {

    const char* name() override { return "Oblique"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "obli";

  protected:
    const char* randomQuote() override { return randomObliqueStrategy(); }
};
ObliqueApp theObliqueApp;

class CognitiveBiasesApp : public QuotesApp {

    const char* name() override { return "Biases"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "bias";

  protected:
    const char* randomQuote() override { return randomCognitiveBias(); }
     const ILI9341_t3_font_t* font() override { return &Arial_12_Bold; }
};
CognitiveBiasesApp theCognitiveBiasesApp;
