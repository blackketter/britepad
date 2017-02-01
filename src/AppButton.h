#ifndef _AppButton_

#include "widgets/Button.h"
#include "apps/BritepadApp.h"
// APPBUTTON
class AppButton : public RoundButton {
  public:
    AppButton() {};
    AppButton(BritepadApp* a) { setApp(a); setColor(screen.blue);}
    ~AppButton() {};
    BritepadApp* getApp() { return (BritepadApp*)getID(); }
    void setApp(BritepadApp* a) { setID((widgetid_t)a); }

    virtual const char* getTitle() { return getApp()->name(); }
    virtual Icon        getIcon()  { return getApp()->getIcon(); }

    virtual color_t     getColor() {
      color_t c = RoundButton::getColor();
      if (getApp()->highlighted()) {
          c = screen.brighten(c, 128);
        }
      return c;
    }
    virtual color_t     getTitleColor() { return Screen::black; }
    virtual font_t      getTitleFont()  { return strchr(getTitle(), '\n') ? Arial_8_Bold : Arial_9_Bold; }
};

// SETTINGSBUTTON
class SettingsButton : public AppButton {
  public:
    SettingsButton(BritepadApp* a, AppMode m) { setApp(a); _appMode = m; setColor(screen.yellow);}
    virtual color_t getColor() {
      color_t c = RoundButton::getColor();
      if (!getApp()->getEnabled(_appMode)) {
          c = screen.darken(c);
        }
      return c;
    }

  private:
    AppMode _appMode;
};

#endif