#ifndef _App_
#define _App_

#include "KeyEvent.h"

enum EventPriority {
    PRIORITY_FIRST = -1000,
    PRIORITY_EARLY = -500,
    PRIORITY_MIDDLE = 0,
    PRIORITY_NORMAL = 500,
    PRIORITY_LAST = 1000,
    PRIORITY_END = 9999,
};

typedef const char* appid_t;

class App {
  public:
    App() { addApp(this); };

    virtual const char* name() = 0;
    virtual appid_t id() = 0;

    inline App* getNextApp() { return _nextApp; }
    inline void setNextApp(App* app) {  _nextApp = app; };

    static App* getFirstApp() { return _appList; }
    static App* getAppByID(appid_t appID);
    static void sortApps();
    static App* findApp(const char* s);

    virtual void init() {};  // initialize app at system startup
    virtual void begin() {}; // called once when an app is launched
    virtual void run() {};   // called periodically while app is launched
    virtual void end();      // called after final run(), lets app clean up, releasing any memory temporarily allocated for runs

    bool isID(appid_t match);

    // give apps an opportuntity to run in the background, useful for processing keyboard events before they go to the currently running app or off to host
    // return if the event is consumed and should not be further processed
    virtual bool event(KeyEvent* key);
    virtual EventPriority eventPriority() { return PRIORITY_NORMAL; };

  protected:
    static const millis_t _maxRunTime = 10;

  private:
    App* _nextApp = nullptr;
    static App* _appList;

    static void setFirstApp(App* a) { _appList = a; }
    static void addApp(App* app) {
      app->setNextApp(_appList);
      _appList = app;
    }
};

#endif


