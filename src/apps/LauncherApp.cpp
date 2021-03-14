#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "ConsoleApp.h"
#include "widgets/AppButton.h"
#include "widgets/ButtonMatrix.h"

class LauncherPage {
  public:
    LauncherPage(const char* name, const char* info, AppType type, AppMode mode, color_t color, LauncherPage* next, BritepadApp* startingWith = nullptr);
    LauncherPage() {};
    virtual ~LauncherPage();
    virtual const char* name() { return _name; }
    const char* info() { return _info; }
    AppType type() { return _type; }
    AppMode mode() { return _mode; }
    color_t color() { return _color; }
    LauncherPage* next() { return _next; }
    void draw();
    bool run(Event* key, LauncherApp* app);
    void setSelected(buttonindex_t i) { _buttons->setSelected(i); }

  protected:
    virtual bool filter(BritepadApp* a);
    virtual int32_t appPos(BritepadApp* a) { return a->getLauncherPosition(); };
    void makeButtons(BritepadApp* startingWith);

    const char* _name;
    const char* _info;
    AppType _type = ANY_TYPE_APP;
    AppMode _mode = ANY_MODE;
    color_t _color;
    LauncherPage* _next = nullptr;
    ButtonMatrix* _buttons = nullptr;
    BritepadApp* _launchOnRelease = nullptr;
    bool _held = false;

    const static int _h_buttons = 4;
    const static int _v_buttons = 3;
    const static int _buttons_per_page = _h_buttons * _v_buttons;
};

LauncherPage::LauncherPage(const char* name, const char* info, AppType type, AppMode mode, color_t color, LauncherPage* next, BritepadApp* startingWith)
 : _name{name}, _info{info}, _type{type}, _mode{mode}, _color{color}, _next{next} {
  // note that makeButtons will create additional launcherpages in case there are too many buttons for a single page
  makeButtons(startingWith);
}

LauncherPage::~LauncherPage() {
  if (_buttons) { delete(_buttons); }
}

void LauncherPage::draw() {
  _buttons->setBounds(screen.clipLeft(), screen.clipTop(), screen.clipWidth(), screen.clipHeight());
  _buttons->draw();
}

bool LauncherPage::filter(BritepadApp* a) {
  return a->isAppType(type());
}

void LauncherPage::makeButtons(BritepadApp* startingWith) {
  if (_buttons) { delete(_buttons); }

  _buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),_v_buttons,_h_buttons,1);
  _buttons->setGap(7);

  // do all the specifically positioned apps, then the default (0) position apps
  for (int specific = 1; specific >= 0; specific--) {
    BritepadApp* a = (BritepadApp*)(startingWith ? startingWith->getNextApp() : BritepadApp::getFirstApp());
    while (a) {
      if (filter(a)) {
        int32_t pos = appPos(a);
        if ((specific && (pos!=BritepadApp::defaultLauncherPosition)) || (!specific && (pos==BritepadApp::defaultLauncherPosition))) {
          if (pos==BritepadApp::defaultLauncherPosition) { pos = 0; }
          while (_buttons->getButton(pos,0)) {
            pos++;
          }
          if (!a->isHidden()) {
            if (pos >= _buttons_per_page) {
              // chain along another page
              _next = new LauncherPage(_name, _info, _type, _mode, _color, _next, a);
              return;
            } else {
              Button* b = nullptr;

              switch (mode()) {
                case KEYBOARD_MODE:
                  b = new KeyboardButton(a);
                  break;
                case MOUSE_MODE:
                  b = new MouseButton(a);
                  break;
                case SCREENSAVER_MODE:
                  b = new ScreensaverButton(a);
                  break;
                case INTERACTIVE_MODE:
                default:
                  b = a->newAppButton();
                  break;
              }
              _buttons->setButton(b,pos);
            }
          }
        }
      }
      a = a->getNextBritepadApp();
    }
  }
}

bool LauncherPage::run(Event* key, LauncherApp* app) {

  if (!_launchOnRelease && (key && (key->released(KEY_SPACE) || key->released(KEY_RETURN)))) {
    _buttons->setHighlighted(false);
    key = nullptr;
  }

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (_launchOnRelease) {
    if ( pad.released(SCREEN_PAD) ||
        ( key && (key->released(KEY_SPACE) ||
                  key->released(KEY_RETURN))
        )
       ) {
      AppMode runMode = _launchOnRelease->canBeSetup() ? SETUP_MODE : mode();
      launcher.launchApp(_launchOnRelease, runMode);
      launcher.resetScreensaver(5*60*1000);  // stay running for up to 5 minutes
      _launchOnRelease = nullptr;
      key = nullptr;
    }
  } else if (
        pad.pressed(LEFT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_LEFT)
     || (key && key->pressed(KEY_PAGE_DOWN))
    ) {
      app->pushPage(DIRECTION_LEFT);
      key = nullptr;
  } else if (pad.pressed(RIGHT_PAD)
     || (pad.getGesture() == GESTURE_SWIPE_RIGHT)
     || (key && key->pressed(KEY_PAGE_UP))
  ) {
    app->pushPage(DIRECTION_RIGHT);
    key = nullptr;
  } else if (
        (pad.getGesture() == GESTURE_SWIPE_UP)
     || (key && key->pressed(KEY_ESC))
      ) {
    launcher.exit();
    key = nullptr;
  }

  if (_buttons->buttonCount()) {

    AppButton* b = (AppButton*)_buttons->releasedButton();

    if (!b && (key && (key->pressed(KEY_SPACE) || key->pressed(KEY_RETURN)))) {
      b = (AppButton*)_buttons->getButton(_buttons->getSelected(),0);
      if (b) {
        b->setHighlighted(true);
      }
      key = nullptr;
    }
    if (b) {
      BritepadApp* launched = b->getApp();
      if (launched->canBeInvisible()) {
        if (_held) {
          _held = false;
        } else {
          console.debugf("Running %s invisibly\n", launched->name());
          launched->setAppMode(INVISIBLE_MODE);
          launched->begin();
          launched->run();
          launched->end();
        }
        b->draw();
      } else {
        AppMode whichMode = mode();
        if (whichMode == INTERACTIVE_MODE || whichMode == ANY_MODE) {
          launcher.launchApp(launched);
          launcher.resetScreensaver(5*60*1000);  // stay running for up to 5 minutes
        } else {
          launched->setEnabled(!launched->getEnabled(whichMode), whichMode);
          b->draw();
        }
      }
      sound.click();
    }
    b = (AppButton*)_buttons->heldButton();
    if (b && b->getApp() && !_held) {
      BritepadApp* launched = b->getApp();
      if (!launched->canBeSetup() && launched->canBeInvisible()) {
        console.debugf("Running %s invisibly on hold\n", launched->name());
        launched->setAppMode(INVISIBLE_MODE);
        launched->begin();
        launched->run();
        launched->end();
       } else if (!_launchOnRelease){
        sound.click();
        app->clearScreen();
        _launchOnRelease = b->getApp();
      }
      _held = true;
    }

    buttonindex_t oldSelection = _buttons->getSelected();
    buttonindex_t i = oldSelection;

    if (key && key->pressed(KEY_UP)) {
      do {
        i -= _h_buttons;
        if (i < 0) { i += _buttons_per_page; }
      } while (_buttons->getButton(i, 0) == nullptr);
      _buttons->setSelected(i);
      key = nullptr;
    }

    if (key && key->pressed(KEY_DOWN)) {
      do {
        i += _h_buttons;
        if (i >= _buttons_per_page) { i -= _buttons_per_page; }
      } while (_buttons->getButton(i, 0) == nullptr);
      _buttons->setSelected(i);
      key = nullptr;
    }

    if (key && key->pressed(KEY_LEFT)) {
      do {
        i--;
        if (i < 0) {
          //app->pushPage(DIRECTION_LEFT);
          i = _buttons_per_page - 1;
        }
      } while (_buttons->getButton(i, 0) == nullptr);
      _buttons->setSelected(i);
      key = nullptr;
    }

    if (key && (key->pressed(KEY_RIGHT)||key->pressed(KEY_TAB))) {
      do {
        i++;
        if (i >= _buttons_per_page) {
          //app->q(DIRECTION_RIGHT);
          i = 0;
        }
      } while (_buttons->getButton(i, 0) == nullptr);
      _buttons->setSelected(i);
      key = nullptr;
    }

    if (oldSelection != i) {
      _buttons->draw(i);
      _buttons->draw(oldSelection);
    }
  }

  if (key && key->released(KEY_EXIT)) {
    if (!_launchOnRelease) {
      launcher.exit();
    }
    key = nullptr;
  }

  if (!key) {
    // if key has been processed, then consume it from the queue
    return true;
  }
  return false;
}

class SearchLauncherPage : public LauncherPage {
  public:
    SearchLauncherPage(String& searchString, const char* info, color_t color, LauncherPage* next, BritepadApp* startingWith = nullptr);
  protected:
    bool filter(BritepadApp* a) override;
    int32_t appPos(BritepadApp* a) override { return BritepadApp::defaultLauncherPosition; };
    const char* name() override;

    String _searchString;
};

SearchLauncherPage::SearchLauncherPage(String& searchString, const char* info, color_t color, LauncherPage* next, BritepadApp* startingWith) {
  _searchString = searchString;
  _info = info;
  _color = color;
  _next = next;

  searchString.toLowerCase();
  makeButtons(startingWith);
}

bool SearchLauncherPage::filter(BritepadApp* a) {
  String title = a->name();
  title.toLowerCase();

  return title.startsWith(_searchString);
}

const char* SearchLauncherPage::name() {
  return _searchString.c_str();
}

/////////////////////////
// LauncherApp Methods //
/////////////////////////
void LauncherApp::makePages() {

  if (_pages) { return; } // pages already made
  LauncherPage* newPage = nullptr;
  if (searchString.length()) {
    newPage = new SearchLauncherPage( searchString, "Search", Screen::darkergreen, newPage);
  } else {
    newPage = new LauncherPage( "Macros",nullptr, MACROS_APP,INTERACTIVE_MODE,Screen::darkergreen, newPage);
    newPage = new LauncherPage( "Apps",nullptr,INTERACTIVE_APP,INTERACTIVE_MODE,Screen::darkergrey, newPage);
    newPage = new LauncherPage( "Timers",nullptr,TIMER_APP,INTERACTIVE_MODE,Screen::darkerblue, newPage);
    newPage = new LauncherPage( "Home",nullptr,KEY_APP,INTERACTIVE_MODE,Screen::darkergreen, newPage);
    newPage = new LauncherPage( "Settings",nullptr,SETTINGS_APP,INTERACTIVE_MODE,Screen::darkeryellow, newPage);
    newPage = new LauncherPage( "Screensavers","Press and hold to test",SCREENSAVER_APP,SCREENSAVER_MODE,Screen::darkeryellow, newPage);
    newPage = new LauncherPage( "Keyboard",nullptr,KEYBOARD_APP,KEYBOARD_MODE,Screen::DarkOrange, newPage);
    newPage = new LauncherPage( "Clocks","Press and hold to test",CLOCK_APP,SCREENSAVER_MODE,Screen::darkerred, newPage);
    newPage = new LauncherPage( "Mice","Press and hold to test",MOUSE_APP,MOUSE_MODE,Screen::black, newPage);
    newPage = new LauncherPage( "Debug",nullptr,DEBUG_APP,INTERACTIVE_MODE,Screen::grey, newPage);
  }
  _pages = newPage;
}

void LauncherApp::freePages() {
  LauncherPage* freed = _pages;
  _pages = nullptr;
  while (freed) {
    LauncherPage* n = freed->next();
    delete freed;
    freed = n;
  }
}

pageindex_t LauncherApp::firstPageOfType(AppType type) {
  pageindex_t index = 0;
  LauncherPage* cur = _pages;
  while (cur) {
    if (cur->type() == type) {
      break;
    }
    cur = cur->next();
    index++;
  }
  return index;
}

//  LAUNCHERAPP
color_t LauncherApp::bgColor() {
  return getCurrentPage()->color();
}

const char* LauncherApp::infoBarText() {
  return getCurrentPage()->info();
}

const char* LauncherApp::statusBarTitle() {
  return getCurrentPage()->name();
}

void LauncherApp::setCurrentPageIndex(pageindex_t n) {
  current_page = n;
}

void LauncherApp::setCurrentPageIndex() {
  setCurrentPageIndex(firstPageOfType(defaultPageAppType));
}

LauncherPage* LauncherApp::getCurrentPage() {
  LauncherPage* s = _pages;
  pageindex_t p = getCurrentPageIndex();
  while(s && p != 0) {
    p--;
    s = s->next();
  }
  if (s == nullptr) {
    s = _pages;
  }
  return s;
}

LauncherApp::LauncherApp() {
}

void LauncherApp::begin() {

  // first, sort the list alphabetically
  BritepadApp::sortApps();

  launcher.resetScreensaver();

  // this should wake up the host, which is great for entering passwords
  launcher.wakeHost();
  makePages();

  if (clock.now() - lastBegin < 2) {
    // launching twice quickly resets to default page
    setCurrentPageIndex();
  } else if (clock.now() - lastRun > resetScreenTimeout) {
    // if we haven't run in a while, reset to the default page
    setCurrentPageIndex();
  } else if (getCurrentPage()->type() == MACROS_APP) {
    // don't jump back to the macros page
    setCurrentPageIndex();
  } else {
    setCurrentPageIndex(getCurrentPageIndex());
  }

  if (pad.touched(SCREEN_PAD)) {
    waitForRelease = true;
  }

  // adjust the current page before beginning
  BritepadApp::begin();

  sound.swipe(DIRECTION_DOWN);

//  console.debugln("start pushfill");
  screen.pushFill(DIRECTION_DOWN, bgColor());

  getCurrentPage()->draw();
  lastBegin = clock.now();
//  console.debugln("done LauncherApp::begin");
}

void LauncherApp::run() {
  Event* key = peekNextEvent();

  if (pad.released(SCREEN_PAD)) { waitForRelease = false; }

  lastRun = clock.now();

  bool consumed = false;

//  if (!waitForRelease && key) {
    consumed = getCurrentPage()->run(key,this);
//  }

  if (!consumed && key && key->character()) {
    if (key->pressed()) {

      char c = key->character();

      if (c == 8) { // backspace
        if (searchString.length()) {
          searchString.remove(searchString.length() - 1);
        }
      } else {
        searchString += c;
      }

      freePages();
      makePages();

      if (searchString.length()) {
        setCurrentPageIndex(0);
        getCurrentPage()->setSelected(0);
      } else {
        setCurrentPageIndex();
      }

      launcher.drawBars();
      clearScreen();
      getCurrentPage()->draw();
    }
    consumed = true;
  }

  // consume any other releases
  if (!consumed && key && key->released()) {
    consumed = true;
  }

  if (!consumed && key && key->code() && key->pressed()) {
    // if the key hasn't been processed, then bail
    launcher.exit();
  }

  if (consumed) {
      getNextEvent();
  }
}

void LauncherApp::goToPage(pageindex_t s) {
  pageindex_t curr = getCurrentPageIndex();
  if (s != curr) {
    direction_t d = s > curr ? DIRECTION_RIGHT : DIRECTION_LEFT;
    setCurrentPageIndex(s);
    sound.swipe(d);
    screen.pushFill(d, bgColor());
    launcher.drawBars();
    getCurrentPage()->draw();
  }
}
void LauncherApp::pushPage(direction_t d) {
    int move = (d == DIRECTION_RIGHT) ? +1 : -1;
    pageindex_t newPage = getCurrentPageIndex() + move;
    if (newPage >= 0 && newPage < pageCount()) {
      goToPage(newPage);
    } else {
      sound.bump();
    }
}

pageindex_t LauncherApp::pageCount() {
  pageindex_t c = 0;
  LauncherPage* curr = _pages;
  while (curr) {
    c++;
    curr = curr->next();
  }
  return c;
}

void LauncherApp::end() {

  if (pad.pressed(TOP_PAD) || audibleExit) {
    sound.swipe(DIRECTION_UP);
  }
  audibleExit = false;

  screen.pushFill(DIRECTION_UP, ((BritepadApp*)(launcher.getLaunchedApp()))->bgColor());
  BritepadApp::end();

  freePages();
  if (searchString.length()) {
    searchString = "";
    setCurrentPageIndex();
    lastRun = 0; // reset launch screen to default
  }
  launcher.resetScreensaver();
}

bool LauncherApp::event(Event* key) {
  bool consume = false;
  if (key->code(KEY_EXIT)) {
    if (key->pressed()) {
      if (events->keyIsDown(MODIFIERKEY_LEFT_SHIFT) && !launcher.currentApp()->isID(ConsoleApp::ID)) {
        launcher.launchApp(ConsoleApp::ID);
      } else {
        launcher.exit();
      }
      audibleExit = true;  // if we're exiting this app, then play exit sound.

    }
    consume = true;
  }

  if (key->code(KEY_LEFT_FN) || key->code(KEY_RIGHT_FN)) {
    if (isCurrentApp()) {
      exit();
      consume = true;
    } else {
//      if (events->keyTapped(KEY_LEFT_FN) || events->keyTapped(KEY_RIGHT_FN)) {
//        launch();
//      }
    }
  }

  return consume;
}

bool LauncherApp::disablesScreensavers() {
    return false;
//  return events->keyIsDown(KEY_RIGHT_FN);
}

LauncherApp theLauncherApp;
