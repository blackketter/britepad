#include "BritepadShared.h"
#include "LauncherApp.h"
#include "ScreensaverApp.h"
#include "ConsoleApp.h"
#include "widgets/AppButton.h"
#include "widgets/ButtonMatrix.h"

keycode_t launchKeys[] = {
KEY_Q, KEY_W, KEY_E, KEY_R,
KEY_A, KEY_S, KEY_D, KEY_F,
KEY_Z, KEY_X, KEY_C, KEY_V
};

class LauncherPage {
  public:
    LauncherPage(const char* name, const char* info, AppType type, AppMode mode, color_t color, LauncherPage* next, BritepadApp* startingWith = nullptr);
    ~LauncherPage();
    const char* name() { return _name; }
    const char* info() { return _info; }
    AppType type() { return _type; }
    AppMode mode() { return _mode; }
    color_t color() { return _color; }
    void setNext(LauncherPage* next) { _next = next; }
    LauncherPage* next() { return _next; }
    void makeButtons(BritepadApp* startingWith);
    void draw();
    bool run(KeyEvent* key, LauncherApp* app);

  protected:
    const char* _name;
    const char* _info;
    AppType _type;
    AppMode _mode;
    color_t _color;
    LauncherPage* _next;
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

void LauncherPage::makeButtons(BritepadApp* startingWith) {
  if (_buttons) { delete(_buttons); }

  _buttons = new ButtonMatrix(screen.clipLeft(),screen.clipTop(),screen.clipWidth(),screen.clipHeight(),_v_buttons,_h_buttons,1);
  _buttons->setGap(7);

  // do all the specifically positioned apps, then the default (0) position apps
  for (int specific = 1; specific >= 0; specific--) {
    BritepadApp* a = britepad.getNextApp(startingWith);
    while (a) {
      if (a->isAppType(type())) {
        britepad.idle();
        int32_t pos = a->getLauncherPosition();
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
                  b = a->newAppButton();
                  break;
                default:
                  break;
              }
              _buttons->setButton(b,pos);
            }
          }
        }
      }
      a = britepad.getNextApp(a);
    }
  }
}

bool LauncherPage::run(KeyEvent* key, LauncherApp* app) {

  for (int i = 0; i < _buttons_per_page; i++) {
    if (key && key->pressed(launchKeys[i]) && _buttons->getButton(i)) {
      _buttons->getButton(i)->setHighlighted(true);
      key = nullptr;
      break;
    }
  }

  if (!_launchOnRelease && (key && (key->released(KEY_SPACE) || key->released(KEY_RETURN)))) {
    _buttons->setHighlighted(false);
    key = nullptr;
  }

  // wait until we release the button to actually launch the press-and-hold screensaver test
  if (_launchOnRelease) {
    if ( pad.released(SCREEN_PAD) ||
        ( key && (key->released(KEY_SPACE) ||
                  key->released(KEY_RETURN) ||
                  key->released(launchKeys[_launchOnRelease->getLauncherPosition()]))
        )
       ) {
      AppMode runMode = _launchOnRelease->canBeSetup() ? SETUP_MODE : mode();
      britepad.launchApp(_launchOnRelease, runMode);
      britepad.resetScreensaver(5*60*1000);  // stay running for up to 5 minutes
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
    britepad.exit();
    key = nullptr;
  } else {
    AppButton* b = (AppButton*)_buttons->releasedButton();

    if (!b) {
      for (int i = 0; i < _buttons_per_page; i++) {
        if (key && key->released(launchKeys[i]) && _buttons->getButton(i)) {
          b = (AppButton*)_buttons->getButton(i);
          b->setHighlighted(false);
          key = nullptr;
          break;
        }
      }
    }

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
          launched->begin(INVISIBLE_MODE);
          launched->run();
          launched->end();
        }
        b->draw();
      } else {
        AppMode whichMode = mode();
        if (whichMode == INTERACTIVE_MODE) {
          britepad.launchApp(launched);
        } else {
          launched->setEnabled(!launched->getEnabled(whichMode), whichMode);
          b->draw();
        }
      }
      sound.click();
    }
    b = (AppButton*)_buttons->heldButton();
    if (b && b->getApp()) {
      BritepadApp* launched = b->getApp();
      if (!launched->canBeSetup() && launched->canBeInvisible()) {
        console.debugf("Running %s invisibly on hold\n", launched->name());
        launched->begin(INVISIBLE_MODE);
        launched->run();
        launched->end();
        _held = true;
      } else {
        sound.click();
        app->clearScreen();
        _launchOnRelease = b->getApp();
      }
    }
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

  if (key && key->released(KEY_EXIT)) {
    if (!_launchOnRelease) {
      britepad.exit();
    }
    key = nullptr;
  }

  if (oldSelection != i) {
    _buttons->draw(i);
    _buttons->draw(oldSelection);
  }

  if (key && key->code() && key->pressed()) {
    // if the key hasn't been processed, then bail
    britepad.exit();
  } else {
    // if key has been processed, then consume it from the queue
    return true;
  }
  return false;
}

/////////////////////////
// LauncherApp Methods //
/////////////////////////
void LauncherApp::makePages() {
  if (_pages) { return; } // pages already made

  LauncherPage* newPage;
  newPage = new LauncherPage( "Macros",nullptr, MACROS_APP,INTERACTIVE_MODE,Screen::darkergreen, nullptr);
  newPage = new LauncherPage( "Apps",nullptr,INTERACTIVE_APP,INTERACTIVE_MODE,Screen::darkergrey, newPage);
  newPage = new LauncherPage( "Timers",nullptr,TIMER_APP,INTERACTIVE_MODE,Screen::darkerblue, newPage);
  newPage = new LauncherPage( "Home",nullptr,KEY_APP,INTERACTIVE_MODE,Screen::darkergreen, newPage);
  newPage = new LauncherPage( "Settings",nullptr,SETTINGS_APP,INTERACTIVE_MODE,Screen::darkeryellow, newPage);
  newPage = new LauncherPage( "Screensavers","Press and hold to test",SCREENSAVER_APP,SCREENSAVER_MODE,Screen::darkeryellow, newPage);
  newPage = new LauncherPage( "Keyboard",nullptr,KEYBOARD_APP,KEYBOARD_MODE,Screen::DarkOrange, newPage);
  newPage = new LauncherPage( "Clocks","Press and hold to test",CLOCK_APP,SCREENSAVER_MODE,Screen::darkerred, newPage);
  newPage = new LauncherPage( "Mice","Press and hold to test",MOUSE_APP,MOUSE_MODE,Screen::black, newPage);
  newPage = new LauncherPage( "Debug",nullptr,DEBUG_APP,INTERACTIVE_MODE,Screen::grey, newPage);

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

LauncherPage* LauncherApp::getCurrentPage() {
  LauncherPage* s = _pages;
  pageindex_t p = getCurrentPageIndex();
  while(s && p != 0) {
    p--;
    s = s->next();
  }
  return s;
}

LauncherApp::LauncherApp() {
}

void LauncherApp::begin(AppMode asMode) {
  console.debugln("start LauncherApp::begin");

  console.debugln("sort apps");
  // first, sort the list alphabetically
  britepad.sortApps();

  // this should wake up the host, which is great for entering passwords
  // but might have some side effects
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_SHIFT);

  console.debugln("makepages");
  makePages();

  console.debugln("start setpage");
  if (clock.now() - lastBegin < 2) {
    // launching twice quickly resets to default page
    setCurrentPageIndex(firstPageOfType(defaultPageAppType));
  } else if (clock.now() - lastRun > resetScreenTimeout) {
    // if we haven't run in a while, reset to the default page
    setCurrentPageIndex(firstPageOfType(defaultPageAppType));
  } else if (getCurrentPage()->type() == MACROS_APP) {
    // don't jump back to the macros page
    setCurrentPageIndex(firstPageOfType(defaultPageAppType));
  } else {
    setCurrentPageIndex(getCurrentPageIndex());
  }

  if (pad.touched(SCREEN_PAD)) {
    waitForRelease = true;
  }

  // adjust the current page before beginning
  BritepadApp::begin(asMode);

  sound.swipe(DIRECTION_DOWN);

  //console.debugln("start pushfill");
  screen.pushFill(DIRECTION_DOWN, bgColor());

  getCurrentPage()->draw();
  lastBegin = clock.now();
  //console.debugln("done LauncherApp::begin");
}

void LauncherApp::run() {
  KeyEvent* key = peekNextEvent();

  if (pad.released(SCREEN_PAD)) { waitForRelease = false; }

  lastRun = clock.now();

  bool consumed = false;

  if (!waitForRelease) {
    consumed = getCurrentPage()->run(key,this);
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
    drawBars();
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

  screen.pushFill(DIRECTION_UP, britepad.getLaunchedApp()->bgColor());
  BritepadApp::end();

  freePages();
}

bool LauncherApp::event(KeyEvent* key) {
  bool consume = false;
  if (key->code(KEY_EXIT)) {
    if (key->pressed()) {
      if (keyEvents.keyIsDown(MODIFIERKEY_LEFT_SHIFT) && !britepad.currentApp()->isID(ConsoleApp::ID)) {
        britepad.launchApp(ConsoleApp::ID);
      } else {
        britepad.exit(); // LauncherApp is what other apps exit to
        britepad.resetScreensaver();
      }
      audibleExit = true;  // if we're exiting this app, then play exit sound.

    }
    consume = true;
  }
  return consume;
}

bool LauncherApp::disablesScreensavers() {
    return false;
//  return keyEvents.keyIsDown(KEY_RIGHT_FN);
}

LauncherApp theLauncherApp;
