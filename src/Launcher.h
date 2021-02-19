#ifndef _Launcher_
#define _Launcher_
#include "App.h"
#include "Console.h"
#include "KeyEventQueue.h"
extern KeyEventQueue* keyEvents;

class Launcher {
  public:
    virtual void begin();  // system setup

    virtual void run();   // event loop calls app run() and idle()

    virtual void idle();   // background processing (esp. for event handling)

    virtual void launchApp(appid_t id);
    virtual void launchApp(App* app);

    App* currentApp() { return _currApp; }
    bool isCurrentApp(App* a) { return currentApp() == a; }
    void exit();

    bool event(KeyEvent* e);
    void timeChanged();

    millis_t lastEventTime() { return keyEvents->lastEventTime(); }

    App* getLaunchedApp() { return _launchedApp; }

  protected:

    App* _currApp = nullptr;
    App* _launchedApp = nullptr;
    App* _lastApp = nullptr;


};
#endif
