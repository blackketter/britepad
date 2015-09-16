#include "FastLED.h"

#include "BritepadShared.h"
#include "BriteLiteApp.h"
#include "Debug.h"

void BriteLiteApp::updateDot(int x, int y) {
  int dotspacing_x = width() / dots_wide;
  int dotspacing_y = ((long)dotspacing_x * 866L) / 1000L;  // .8660 is sqrt(3)/2

  int r = (dotspacing_x * 8) / 10 / 2;  // dots are 80% of space
  int center_x = x * dotspacing_x + dotspacing_x/2 + (y%2 ? dotspacing_x / 2 : 0);
  int center_y = y * dotspacing_y + dotspacing_y/2;

  // don't draw the ones that are pushed too far right
  if (center_x + r < right() && center_y + r < bottom()) {
    screen.fillCircle(left() + center_x, top() + center_y, r, dots[x*dots_wide+y]);
  }
}

BritepadApp* BriteLiteApp::run(void) {

  switch (random(8)) {
    case 0:
      cursor_x++;
      cursor_y--;
      break;
    case 1:
      cursor_x++;
      break;
    case 2:
      cursor_x++;
      cursor_y++;
      break;
    case 3:
      cursor_y++;
      break;
    case 4:
    case 6:
    case 7:
      cursor_x--;
      break;
    case 5:
      cursor_y--;
      break;
  }
  cursor_y = min(dots_high-1,max(0,cursor_y));
  cursor_x = min(dots_wide-1,max(0,cursor_x));

  setDot(cursor_x, cursor_y, lastColor++);

  return STAY_IN_APP;
}
