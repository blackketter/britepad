#if TEENSY==1

#include "BritepadShared.h"
#include "MousePad.h"
#include "usb_mouse.h"

void MousePad::begin() {
  Mouse.begin();
}

void MousePad::end() {
  Mouse.release();
  Keyboard.releaseAll();
}

void MousePad::run() {

  // todo: make these sounds more descriptive
  if (pad.pressed(RIGHT_PAD)) { sound.click(); }
  if (pad.released(RIGHT_PAD)) { sound.click(); }
  if (pad.pressed(LEFT_PAD)) { sound.click(); }
  if (pad.released(LEFT_PAD)) { sound.click(); }

  // right panel
  if (pad.changed(RIGHT_PAD)) {
    if (pad.touched(RIGHT_PAD)) {
//      console.debugln("mouse press right panel");
    } else {
//      console.debugln("mouse release right panel");
      Keyboard.release(MODIFIERKEY_CTRL);
    }

    if (pad.touched(RIGHT_PAD)) {
      // right has been pressed
//      console.debugln("right panel pressed");
      // if the left is down, send a tab
      if (pad.touched(LEFT_PAD)) {
        Keyboard.press(MODIFIERKEY_GUI);
        Keyboard.press(KEY_TAB);
        Keyboard.release(KEY_TAB);
      }
      // always hold down ctrl
      Keyboard.press(MODIFIERKEY_CTRL);
    } else {
//      console.debugln("right panel released");
      // always release ctrl
      Keyboard.release(MODIFIERKEY_CTRL);

      if (!pad.touched(LEFT_PAD)) {
        Keyboard.release(MODIFIERKEY_GUI);
      }
    }
  }

  // bottom panel
  if (pad.pressed(BOTTOM_PAD)) {
    Mouse.press();
    pressed = true;
    sound.click();
  } else if (pad.released(BOTTOM_PAD)) {
    Mouse.release();
    pressed = false;
    sound.click();
  }

  // left panel
  if (pad.pressed(LEFT_PAD)) {
    // left has been pressed
//    console.debugln("Left panel pressed");
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

  if (pad.released(LEFT_PAD)) {
    if (!pad.touched(RIGHT_PAD)) {
      Keyboard.release(MODIFIERKEY_GUI);
    }
  }


  if (pad.touched(SCREEN_PAD)) {
    // touch down

    if (scrollMode) {
      // we are in scroll mode
      console.debugln("scroll mode scrolling");
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

          accumScroll += pad.deltay();

          // limit scroll messages to max 25ms intervals and some movement
          if (pad.time() - lastScroll > scrollInterval && abs(accumScroll) > scrollFactor) {
            int8_t mouseScrollUnits = accumScroll/scrollFactor;
            Mouse.scroll(-mouseScrollUnits); // negative because we use new natural scrolling
            // todo: notify scroll
            accumScroll -= mouseScrollUnits*scrollFactor;
            lastScroll = pad.time();
          }
        }
      } else if (!pad.pressed(SCREEN_PAD)) {
        if (isAbsolute()) {
          Mouse.screenSize(screen_h, screen_v, isMac);

          // scale down the pad position to put a dead margin (in percent) around the edge
          uint16_t leftedge = (uint32_t)pad.getWidth() * absMargin / 100;
          uint16_t rightedge = pad.getWidth() - leftedge;
          uint16_t width = rightedge - leftedge;

          uint16_t topedge = (uint32_t)pad.getHeight() * absMargin / 100;
          uint16_t bottomedge = pad.getHeight() - topedge;
          uint16_t height = bottomedge - topedge;

          int16_t x,y;

          x = (int32_t)(pad.x() - leftedge) * screen_h / width;
          y = (int32_t)(pad.y() - topedge) * screen_v / height;

          if (x < 0) { x = 0; }
          if (x >= screen_h) { x = screen_h - 1; }

          if (y < 0) { y = 0; }
          if (y >= screen_v) { y = screen_v - 1; }

          console.debugf("pad.x: %d, pad.y: %d, x: %d, y: %d\n", pad.x(), pad.y(), x, y);
          Mouse.moveTo(x, y);
        } else {
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
          console.debugf("<move: %d, %d>",deltax,deltay);
#endif
        }
      }
    }


  } else {
    // screen not touched

    // screen tap released
    if (pad.released(SCREEN_PAD)) {
      millis_t downtime = pad.time() - pad.lastDownTime(SCREEN_PAD);

      if ( (downtime < MOUSE_TAP_DUR) && (abs(pad.lastDownX() - pad.x()) < MOUSE_TAP_MOVEMENT_MAX && (abs(pad.lastDownY() - pad.y()) < MOUSE_TAP_MOVEMENT_MAX)) ) {

        if (pad.x() > (pad.getWidth() - SCROLL_EDGE_MARGIN)) {
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
        } else {
          if (pressed) {
            Mouse.release();
            pressed = false;
           // todo: notify mouse up
          }
          Mouse.press();
          pressed = true;
          sound.click();
          // todo: notify mouse down
        }
      }

    } else {
      if (pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_TAP_UP_DUR) {
        bool releaseDrag = pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_RELEASE_DRAG_DUR;
        bool noDrag = pad.time() - pad.lastDownTime(SCREEN_PAD) < MOUSE_DRAG_DUR;
        if (noDrag || releaseDrag) {
          if (pressed) {
            if (releaseDrag) {
              sound.click();
            }
            // todo: notify mouse up
            Mouse.release();
            pressed = false;
//            console.debugln("mouse release after timeout");
          }
          scrollMode = false;
        }
      }
    }
  }
}
#endif

