#ifndef _AppButton_

#include "widgets/Button.h"
#include "apps/BritepadApp.h"
// APPBUTTON
class AppButton : public RoundButton {
  public:
    AppButton() {};
    AppButton(BritepadApp* a) { setApp(a); }
    ~AppButton() {};
    BritepadApp* getApp() { return (BritepadApp*)getID(); }
    void setApp(BritepadApp* a) { setID((widgetid_t)a); }

    virtual const char* getTitle() { return getApp()->name(); }
    virtual Icon        getIcon()  { return getApp()->getIcon(); }

    virtual color_t     getColor() {
      color_t c = getApp()->appButtonColor();
      if (getApp()->highlighted()) {
          c = screen.brighten(c, 128);
        }
      return c;
    }
    virtual color_t     getTitleColor() { return Screen::black; }
    virtual font_t      getTitleFont()  { return strchr(getTitle(), '\n') ? Arial_8_Bold : Arial_9_Bold; }
    virtual AppMode     getAppMode()    { return INTERACTIVE_MODE; }
};

// SETTINGSBUTTON - special buttons for settings rather than launching apps
class SettingsButton : public AppButton {
  public:
     virtual color_t getColor() {
      color_t c = RoundButton::getColor();
      if (!getApp()->getEnabled(getAppMode())) {
          c = screen.darken(c);
        }
      return c;
    }
};

class MouseButton : public SettingsButton {
  public:
    MouseButton(BritepadApp* a) { setApp(a); setColor(screen.yellow);}
    virtual AppMode getAppMode() { return MOUSE_MODE; }
};

class ScreensaverButton : public SettingsButton {
  public:
    ScreensaverButton(BritepadApp* a) { setApp(a); setColor(screen.green);}
    virtual AppMode getAppMode() { return SCREENSAVER_MODE; }
};
#endif
