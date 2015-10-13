#include "FastLED.h"
#include "BritepadShared.h"
#include "AnalogClockApp.h"
#include "Debug.h"
#include "Clock.h"

const coord_t sec_spacing = 5;
const coord_t min_spacing = 5;
const coord_t hr_spacing = 5;

const coord_t sec_dot_r = 5;
const coord_t min_dot_r = 10;
const coord_t hr_dot_r = 15;

coord_t sec_length;
coord_t min_length;
coord_t hr_length;

void AnalogClockApp::begin(AppMode asMode) {
  ClockApp::begin(asMode);
  coord_t x = screen.clipMidWidth();
  coord_t y = screen.clipMidHeight();
  coord_t r = min(x,y);

  screen.fillCircle(x,y,r-1,sec_bg);

  r -= (sec_dot_r+sec_spacing);
  sec_length = r;
  r -= (sec_dot_r+sec_spacing);

  screen.fillCircle(x,y,r-1,min_bg);

  r -= (min_dot_r+min_spacing);
  min_length = r;
  r -= (min_dot_r+min_spacing);

  screen.fillCircle(x,y,r-1,hr_bg);
  r -= (hr_dot_r+hr_spacing);
  hr_length = r;
  r -= (hr_dot_r+hr_spacing);

  screen.fillCircle(x,y,r-1,center_bg);

  last_min_x = screen.offscreen;
  last_min_y = screen.offscreen;
  last_hr_x = screen.offscreen;
  last_hr_y = screen.offscreen;
  last_sec_x = screen.offscreen;
  last_sec_y = screen.offscreen;

}

void AnalogClockApp::dotpos(uint16_t theta, coord_t r, coord_t& x, coord_t& y) {
  coord_t center_x = screen.clipMidWidth();
  coord_t center_y = screen.clipMidHeight();

  x = center_x + ((long)r*sin16(theta))/32768L;
  y = center_y - ((long)r*cos16(theta))/32768L;
}

void AnalogClockApp::update(void) {
    coord_t x, y;
    uint16_t hr_theta, min_theta, sec_theta, millis_theta;

    uint8_t now_sec = clock.second();
    long now_millis = 0;

    if (now_sec != last_sec) {
      millis_offset = clock.millis();
      now_millis = 0;
      last_sec = now_sec;
    } else {
      now_millis = clock.millis() - millis_offset;
    }

    millis_theta = ((long)now_millis * 65536L)/1000L;

    sec_theta = ((long)now_sec * 65536L)/60L + millis_theta/60;

    min_theta = ((long)clock.minute() * 65536L)/60L + sec_theta/60;

    long hr  = clock.hourFormat12();
    if (hr==12) { hr=0; }

    hr_theta = (hr * 65536L) / 12L + min_theta/12;

    dotpos(hr_theta, hr_length, x, y);
    if (x != last_hr_x || y != last_hr_y) {
      screen.fillCircle(last_hr_x,last_hr_y, hr_dot_r, hr_bg);
      screen.fillCircle(x,y, hr_dot_r, hr_fg);
      last_hr_x = x;
      last_hr_y = y;
    }

    dotpos(min_theta, min_length, x, y);
    if (x != last_min_x || y != last_min_y) {
      screen.fillCircle(last_min_x,last_min_y, min_dot_r, min_bg);
      screen.fillCircle(x,y, min_dot_r, min_fg);
      last_min_x = x;
      last_min_y = y;
    }

    dotpos(sec_theta, sec_length, x, y);
    if (x != last_sec_x || y != last_sec_y) {
      screen.fillCircle(last_sec_x,last_sec_y, sec_dot_r, sec_bg);
      screen.fillCircle(x,y, sec_dot_r, sec_fg);
      last_sec_x = x;
      last_sec_y = y;
    }
}
