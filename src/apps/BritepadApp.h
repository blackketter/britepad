#ifndef _BritepadApp_
#define _BritepadApp_

#include "App.h"
#include "widgets/Icon.h"

class AppButton;

class BritepadApp : public App {
  public:

    void begin(AppMode asMode) override;  // initialize app state and draw first screen, allocate any memory needed while running

    void switchAppMode(AppMode asMode) override;  // called when switching between modes

    KeyEvent* getNextEvent();
    KeyEvent* peekNextEvent();

    bool timeVisible() { return displaysClock() || displaysStatusBar(); }

    virtual int32_t getLauncherPosition() { return _launcherPosition; }
    virtual void setLauncherPosition(int32_t p) { _launcherPosition = p; }
    virtual AppButton* newAppButton();
    virtual color_t appButtonColor() { return screen.CadetBlue; }

    virtual bool isHidden() { return false; }

    virtual bool isInvisible() { return false; };

    virtual color_t bgColor() { return screen.black; }  // background color of app screen

    virtual void clearScreen();

    virtual bool displaysStatusBar() { return true; };  // apps show status bar by default
    virtual color_t statusBarBGColor() { return screen.mix(bgColor(), screen.grey); }  // bgcolor of status bar
    virtual color_t statusBarFGColor() { return screen.luminance(statusBarBGColor()) > 127 ? screen.black : screen.white; } // color of text, graphics on status bar
    virtual const char* statusBarTitle() { return name(); }

    virtual bool displaysInfoBar() { return infoBarText() != nullptr; };
    virtual color_t infoBarBGColor() { return bgColor(); }
    virtual color_t infoBarFGColor() { return screen.luminance(infoBarBGColor()) > 127 ? screen.grey : screen.lightgrey; }  // info text is faded, by default
    virtual const char* infoBarText() { return nullptr; }

    static const int32_t defaultLauncherPosition = -1;

    virtual Icon getIcon() { return nullptr; };
    void setClipRect(coord_t x, coord_t y, coord_t w, coord_t h);  // resets clip rect to content area

    virtual bool highlighted() { return wantsToRun(); }  // by default, apps are highlighted if they want to be displayed
    virtual bool isScreenApp() { return true; }

// TODO - move these to base class
    void delay(millis_t d);  // an idle aware delay
    bool timeToLeave();

  protected:

    int32_t _launcherPosition = defaultLauncherPosition;

};

#endif


