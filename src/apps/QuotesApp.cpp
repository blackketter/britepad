#include "BritepadShared.h"
#include "ObliqueStrategies.h"
#include "JennyHolzer.h"
#include "CognitiveBiases.h"
#include "ScreensaverApp.h"

class QuotesApp : public ScreensaverApp {
  public:
    void begin() override { lastDrawn = 0; }

    void run() override {
      if (lastDrawn == 0) {
        clearScreen();
        screen.setFont(font());
        screen.setTextColor(screen.white, bgColor());
        screen.setCursor(screen.clipMidWidth(), screen.clipMidHeight());
        screen.setTextAlign((alignment_t)(ALIGN_VCENTER|ALIGN_HCENTER));

        const char* quote = randomQuote();
        if (quote == nullptr) {
          console.debugln("NULL QUOTE!");
        } else {
          console.debugf("Quote: %s\n",quote);
        }

        String wrapped;
        screen.softWrapText(wrapped, quote);
        screen.drawString(wrapped);
        screen.setTextAlign();

        lastDrawn = Uptime::millis();
      }
    }
    
  protected:
    virtual const ILI9341_t3_font_t* font() { return &Arial_20_Bold; }
    millis_t lastDrawn = 0;
    virtual const char* const* quoteList() { return nullptr; }
    virtual const char* randomQuote() {
      const char* const* list = quoteList();
      if (list == nullptr) return "";

      const char* const* currQuote = list;
      int count =0;
      while (*currQuote) {
        count++;
        currQuote++;
      }
      return list[random(count)];
    };

};

class HolzerApp : public QuotesApp {

    const char* name() override { return "Holzer"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "holz";

  protected:
    const char* const* quoteList() override { return jennyHolzerList; }
};
HolzerApp theHolzerApp;

class ObliqueApp : public QuotesApp {

    const char* name() override { return "Oblique"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "obli";

  protected:
    const char* const* quoteList() override { return obliqueStrategiesList; }
};
ObliqueApp theObliqueApp;

class CognitiveBiasesApp : public QuotesApp {

    const char* name() override { return "Biases"; };
    appid_t id() override { return ID; };
    static constexpr appid_t ID = "bias";

  protected:
    const char* const* quoteList() override { return cognitiveBiasesList; }
     const ILI9341_t3_font_t* font() override { return &Arial_12_Bold; }
};
CognitiveBiasesApp theCognitiveBiasesApp;
