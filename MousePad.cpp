#include "BritepadShared.h"
#include "MousePad.h"
#include "Debug.h"

// Mouse parameters
//  move maximum number of points
#define MOUSE_MAX_MOVE (100)

// a tap is less than this many millis
#define MOUSE_TAP_DUR (150)
#define MOUSE_TAP_UP_DUR (200)
#define MOUSE_DRAG_DUR (300)
#define MOUSE_RELEASE_DRAG_DUR (1000)

#define SCROLL_EDGE_MARGIN (10)

void MousePad::begin() {
}

void MousePad::end() {
  Mouse.release();
  Keyboard.releaseAll();
}

void MousePad::run(void) {

  // todo: make these sounds more descriptive
  if (pad.down(RIGHT_PAD)) { sound.click(); }
  if (pad.up(RIGHT_PAD)) { sound.click(); }
  if (pad.down(LEFT_PAD)) { sound.click(); }
  if (pad.up(LEFT_PAD)) { sound.click(); }

  // right panel
  if (pad.changed(RIGHT_PAD)) {
    if (pad.touched(RIGHT_PAD)) {
//      DEBUG_LN("mouse press right panel");
    } else {
//      DEBUG_LN("mouse release right panel");
      Keyboard.release(MODIFIERKEY_CTRL);
    }

    if (pad.touched(RIGHT_PAD)) {
      // right has been pressed
//      DEBUG_LN("right panel pressed");
      // if the left is down, send a tab
      if (pad.touched(LEFT_PAD)) {
        Keyboard.press(MODIFIERKEY_GUI);
        Keyboard.press(KEY_TAB);
        Keyboard.release(KEY_TAB);
      }
      // always hold down ctrl
      Keyboard.press(MODIFIERKEY_CTRL);
    } else {
//      DEBUG_LN("right panel released");
      // always release ctrl
      Keyboard.release(MODIFIERKEY_CTRL);

      if (!pad.touched(LEFT_PAD)) {
        Keyboard.release(MODIFIERKEY_GUI);
      }
    }
  }

  // bottom panel
  if (pad.down(BOTTOM_PAD)) {
    if (!mouseLock) {
      mouseLock = true;
      Mouse.press();
    } else {
      mouseLock = false;
      Mouse.release();
    }
    sound.click();
  }

  // left panel
  if (pad.down(LEFT_PAD)) {
    // left has been pressed
//    DEBUG_LN("Left panel pressed");
    Keyboard.press(MODIFIERKEY_GUI);
    if (pad.touched(RIGHT_PAD)) {
      // release ctrl so we can command tab
      Keyboard.release(MODIFIERKEY_CTRL);

      // tap left and right is already on
      Keyboard.press(MODIFIERKEY_SHIFT);
      Keyboard.press(KEY_TAB);
      Keyboard.release(KEY_TAB);
      Keyboard.release(MODIFIERKEY_SHIFT);

      // restore ctrl key
      Keyboard.press(MODIFIERKEY_CTRL);
    }
  }

  if (pad.up(LEFT_PAD)) {
    if (!pad.touched(RIGHT_PAD)) {
      Keyboard.release(MODIFIERKEY_GUI);
    }
  }


  if (pad.touched(SCREEN_PAD)) {
    // touch down

    if (scrollMode) {
      // we are in scroll mode
      DEBUG_LN("scroll mode scrolling");
      if (pad.y() < pad.getHeight()/2) {
        Keyboard.press(KEY_PAGE_UP);
        Keyboard.release(KEY_PAGE_UP);
//  todo: notify pageup
      } else {
        Keyboard.press(KEY_PAGE_DOWN);
        Keyboard.release(KEY_PAGE_DOWN);
//  todo: notify pagedown
      }
    }

    // send a move message only if we actually move
    if (pad.deltax() != 0 || pad.deltay() != 0) {
      // if the touch is in the edge, then we scroll
      if (pad.x() > (pad.getWidth() - SCROLL_EDGE_MARGIN)) {
        if (pad.deltay() != 0) {
          static millis_t lastScroll = pad.time();
          static int16_t accumScroll = 0;
          const int16_t scrollFactor = 16;

          accumScroll += pad.deltay();

//          DEBUG_PARAM_LN("deltay", pad.deltay());
          // limit scroll messages to max 25ms intervals and some movement
          if (pad.time() - lastScroll > 25 && abs(accumScroll) > scrollFactor) {
            int8_t mouseScrollUnits = accumScroll/scrollFactor;
            Mouse.scroll(-mouseScrollUnits); // negative because we use new natural scrolling
            // todo: notify scroll
//            DEBUG_PARAM_LN("Scroll", mouseScrollUnits);
            accumScroll -= mouseScrollUnits*scrollFactor;
            lastScroll = pad.time();
          }
        }
      } else if (!pad.down(SCREEN_PAD)) {

        // otherwise we move the mouse, if this isn't a down transition
        // scale up movement
        int deltax = pad.deltax() * 2;
        int deltay = pad.deltay() * 2;
        // accelerate
        if (abs(deltax) > 20 || abs(deltay) > 20) {
          deltax *= 4;
          deltay *= 4;
        } else if (abs(deltax) > 10 || abs(deltay) > 10) {
          deltax *= 3;
          deltay *= 3;
        } else if (abs(deltax) > 5 || abs(deltay) > 5) {
          deltax *= 2;
          deltay *= 2;
        }

        if (abs(deltax) > MOUSE_MAX_MOVE) {
          deltax = deltax > 0 ? MOUSE_MAX_MOVE : -MOUSE_MAX_MOVE;
        }

        if (abs(deltay) > MOUSE_MAX_MOVE) {
          deltay = deltay > 0 ? MOUSE_MAX_MOVE : -MOUSE_MAX_MOVE;
        }
        Mouse.move(deltax, deltay);
        // todo: notify mouse move deltax/deltay
#if DEBUG_TRACKING
        // Print out the mouse movements
        DEBUG("<move:"); DEBUG(deltax);
        DEBUG(", "); DEBUG(deltay);     DEBUG(">");
#endif
      }
    }


  } else {
    // screen not touched

    // screen tap released
    if (pad.up(SCREEN_PAD)) {
      long downtime = pad.time() - pad.lastDownTime(SCREEN_PAD);
//      DEBUG_PARAM_LN("Downtime:", downtime);

      if ( (downtime < MOUSE_TAP_DUR) && (abs(pad.lastDownX() - pad.x()) < 20 && (abs(pad.lastDownY() - pad.y()) < 20)) ) {

        if (pad.x() > (pad.getWidth() - SCROLL_EDGE_MARGIN)) {
//          DEBUG_PARAM_LN("pad.x()",pad.x());
          if (pad.y() < pad.getHeight()/2) {
            Keyboard.press(KEY_PAGE_UP);
            Keyboard.release(KEY_PAGE_UP);
            // todo: notify page up
          } else {
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.release(KEY_PAGE_DOWN);
            // todo: notify page down
          }
          scrollMode = true;
//          DEBUG_LN("scrollMode on");
        } else {
          if (!mouseLock) {
            if (Mouse.isPressed()) {
              Mouse.release();
  //            DEBUG_LN("mouse release in order to press after touch up");
              // todo: notify mouse up
            }
            Mouse.press();
            sound.click();
  //          DEBUG_LN("mouse press after touch up");
            // todo: notify mouse down
          }
        }
      }

    } else {
      if (pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_TAP_UP_DUR) {
        bool releaseDrag = pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_RELEASE_DRAG_DUR;
        bool noDrag = pad.time() - pad.lastDownTime(SCREEN_PAD) < MOUSE_DRAG_DUR;
        if (!mouseLock && (noDrag || releaseDrag)) {
          if (Mouse.isPressed()) {
            if (releaseDrag) {
              sound.click();
            }
            // todo: notify mouse up
            Mouse.release();
//            DEBUG_LN("mouse release after timeout");
          }
          scrollMode = false;
        }
      }
    }
  }
}

