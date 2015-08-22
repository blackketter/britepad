#ifndef BPApp
#define BPApp

class BPApp {
  public:
    BPApp() {};
    virtual void init(void) {};
    virtual void end(void) {};
    virtual void run(void) {};
    virtual const char* name(void) { return 0; };
    virtual bool isScreensaver(void) { return false; };
};

#endif
