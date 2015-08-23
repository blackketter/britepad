#include "Britepad.h"
#include "MouseApp.h"
#include "Screen.h"
#include "Debug.h"


// Size of the color selection boxes and the paintbrush size
#define PENRADIUS 10

// Mouse parameters
//  move maximum number of points
#define MOUSE_MAX_MOVE (100)

// a tap is less than this many millis
#define MOUSE_TAP_DUR (100)
#define MOUSE_TAP_UP_DUR (200)
#define MOUSE_DRAG_DUR (300)
#define MOUSE_RELEASE_DRAG_DUR (1000)

#define SCROLL_EDGE_MARGIN (10)

void MouseApp::run(void) {
  static int scroll_mode = 0;

  // right panel
  if (pad.changed(RIGHT_PAD)) {
    if (pad.touched(RIGHT_PAD)) {
      DEBUG_LN("mouse press right panel");
    } else {
      DEBUG_LN("mouse release right panel");
      Keyboard.release(MODIFIERKEY_CTRL);
    }


    if (pad.touched(RIGHT_PAD)) {
      // right has been pressed
      DEBUG_LN("right panel pressed");
      // if the left is down, send a tab
      if (pad.touched(LEFT_PAD)) {
        Keyboard.press(MODIFIERKEY_GUI);
        Keyboard.press(KEY_TAB);
        Keyboard.release(KEY_TAB);
      }
      // always hold down ctrl
      Keyboard.press(MODIFIERKEY_CTRL);
    } else {
      DEBUG_LN("right panel released");
      // always release ctrl
      Keyboard.release(MODIFIERKEY_CTRL);

      if (!pad.touched(LEFT_PAD)) {
        Keyboard.release(MODIFIERKEY_GUI);
      }
    }
  }

  // bottom panel
  if (pad.down(BOTTOM_PAD)) {
    if (!Mouse.isPressed()) {
      Mouse.press();
      screen.fillScreen(screen.red);
    }
  }
  if (pad.up(BOTTOM_PAD)) {
    if (Mouse.isPressed()) {
      Mouse.release();
      screen.fillScreen(screen.black);
    }
  }


  // left panel
  if (pad.down(LEFT_PAD)) {
    // left has been pressed
    DEBUG_LN("Left panel pressed");
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
    DEBUG_LN("Left panel released");
    if (!pad.touched(RIGHT_PAD)) {
      Keyboard.release(MODIFIERKEY_GUI);
    }
  }

  const int feedbackWidth = 4;
  // show feedback on touch panels
  if (pad.changed(RIGHT_PAD)) {
    screen.fillRect(screen.width()-feedbackWidth,0,feedbackWidth,screen.height()-1, pad.touched(RIGHT_PAD) ? screen.red : backgroundColor);
  }

  if (pad.changed(LEFT_PAD)) {
    screen.fillRect(0,0,feedbackWidth, screen.height()-1, pad.touched(LEFT_PAD) ? screen.red : backgroundColor);
  }

  if (pad.changed(BOTTOM_PAD)) {
    screen.fillRect(0,screen.height()-feedbackWidth,screen.width()-1,feedbackWidth, pad.touched(BOTTOM_PAD) ? screen.red : backgroundColor);
  }


  if (pad.touched(SCREEN_PAD)) {
    // touch down
#if DEBUG_TRACKING
    DEBUG_START("Tracking: ");
   // Print out the raw coordinates
    DEBUG("[raw:"); DEBUG(pad.x());    DEBUG(","); DEBUG(pad.y());
    DEBUG_END("]");
#endif
    screen.setCursor(0,0);
    screen.setTextColor(currentColor, 0);
    screen.setTextSize(2);
    screen.drawTextF("%3d, %3d", pad.x(), pad.y());

    int radius = PENRADIUS;
    if (Mouse.isPressed()) {
      radius *= 2;
    }

    if ((pad.y()+radius) < screen.height()) {

      screen.fillCircle(pad.x(), pad.y(), radius, currentColor);
      currentColor++;
    }

    // screen tap down
    if (pad.touched(SCREEN_PAD)) {

      // send a move message only if we actually move
      if (pad.deltax() != 0 || pad.deltay() != 0) {
        // if the right touch is down, then we scroll
        if (scroll_mode || pad.touched(RIGHT_PAD) || pad.x() > (screen.width() - SCROLL_EDGE_MARGIN)) {
          if (pad.deltay() != 0) {
            static long lastScroll = pad.time();
            static int16_t accumScroll = 0;
            const int16_t scrollFactor = 16;

            accumScroll += pad.deltay();

            DEBUG_PARAM_LN("deltay", pad.deltay());
            // limit scroll messages to max 25ms intervals and some movement
            if (pad.time() - lastScroll > 25 && abs(accumScroll) > scrollFactor) {
              int8_t mouseScrollUnits = accumScroll/scrollFactor;
              Mouse.scroll(-mouseScrollUnits); // negative because we use new natural scrolling
              DEBUG_PARAM_LN("Scroll", mouseScrollUnits);
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
#if DEBUG_TRACKING
          // Print out the mouse movements
          DEBUG("<move:"); DEBUG(deltax);
          DEBUG(", "); DEBUG(deltay);     DEBUG(">");
#endif
          screen.drawLine(screen.width()/2, screen.height()/2, screen.width()/2+deltax, screen.height()/2+deltay, ~currentColor);
        }
      }
    }

  } else {
    // screen not touched

    // screen tap released
    if (pad.up(SCREEN_PAD)) {
       if ((pad.time() - pad.lastDownTime(SCREEN_PAD) < MOUSE_TAP_DUR) && (abs(pad.lastDownX() - pad.x()) < 20 && (abs(pad.lastDownY() - pad.y()) < 20)) ) {

        if (pad.x() > (screen.width() - SCROLL_EDGE_MARGIN)) {
          DEBUG_PARAM_LN("pad.x()",pad.x());
          screen.fillScreen(backgroundColor);
          if (pad.y() < screen.height()/2) {
            Keyboard.press(KEY_PAGE_UP);
            Keyboard.release(KEY_PAGE_UP);
            screen.fillRect(0,0,screen.width(), screen.height()/2, currentColor);
          } else {
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.release(KEY_PAGE_DOWN);
            screen.fillRect(0,screen.height()/2, screen.width(), screen.height()/2, currentColor);
          }
        } else {
          if (Mouse.isPressed() && !pad.touched(BOTTOM_PAD)) {
            Mouse.release();
            screen.fillScreen(screen.black);
            DEBUG_LN("mouse release in order to press after touch up");
          }
          Mouse.press();
          DEBUG_LN("mouse press after touch up");
        }
      }

    } else {
      if ( !pad.touched(LEFT_PAD) && pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_TAP_UP_DUR) {
        if (Mouse.isPressed()) {
          screen.fillCircle(pad.x(), pad.y(), PENRADIUS*2, currentColor);
        }

        if (pad.time() - pad.lastDownTime(SCREEN_PAD) < MOUSE_DRAG_DUR || pad.time() - pad.lastUpTime(SCREEN_PAD) > MOUSE_RELEASE_DRAG_DUR) {
          if (Mouse.isPressed() && !pad.touched(BOTTOM_PAD)) {
            Mouse.release();
            screen.fillScreen(screen.black);
            screen.fillCircle(pad.x(), pad.y(), PENRADIUS*2, ~currentColor);
            DEBUG_LN("mouse release after timeout");
          }

          scroll_mode = 0;

        }
      }
    }
  }
}

