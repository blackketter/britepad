#ifndef _ConsoleApp_
#define _ConsoleApp_
#include "BritepadApp.h"
#include "widgets/TerminalWidget.h"

class ConsoleApp : public BritepadApp {
  public:
    void begin(AppMode asMode);
    void end();
    void run();

    virtual bool disablesScreensavers() { return true; }
    virtual bool usesKeyboard() { return true;}
    virtual BritepadApp* exitsTo() { return A_SCREENSAVER_APP; }

    const char* name() { return "Console"; };
    appid_t id() { return ID; };
    static constexpr appid_t ID = "cons";

    AppType getAppType() { return INTERACTIVE_APP; }

  private:

  protected:
    TerminalWidget* _terminal = nullptr;
    Stream* _origPort = nullptr;
    virtual void setPort();
    virtual void endPort();
  };

#endif
