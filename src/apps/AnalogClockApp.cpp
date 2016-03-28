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

void AnalogClockApp::begin() {
  ClockApp::begin();
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

void AnalogClockApp::update() {
    coord_t x, y;
    uint16_t hr_theta, min_theta, sec_theta, millis_theta;

    uint8_t now_sec = clock.second();
    uint16_t now_millis = clock.fracMillis();

    millis_theta = (now_millis * 65536L)/1000L;

    sec_theta = ((int32_t)now_sec * 65536L)/60L + millis_theta/60;

    min_theta = (clock.minute() * 65536L)/60L + sec_theta/60;

    int32_t hr  = clock.hourFormat12();
    if (hr==12) { hr=0; }

    hr_theta = (hr * 65536L) / 12L + min_theta/12;

    screen.polarToRect(hr_theta, hr_length, x, y);
    if (x != last_hr_x || y != last_hr_y) {
      screen.fillCircle(last_hr_x,last_hr_y, hr_dot_r, hr_bg);
      screen.fillCircle(x,y, hr_dot_r, hr_fg);
      last_hr_x = x;
      last_hr_y = y;
    }

    screen.polarToRect(min_theta, min_length, x, y);
    if (x != last_min_x || y != last_min_y) {
      screen.fillCircle(last_min_x,last_min_y, min_dot_r, min_bg);
      screen.fillCircle(x,y, min_dot_r, min_fg);
      last_min_x = x;
      last_min_y = y;
    }

    screen.polarToRect(sec_theta, sec_length, x, y);
    if (x != last_sec_x || y != last_sec_y) {
      screen.fillCircle(last_sec_x,last_sec_y, sec_dot_r, sec_bg);
      screen.fillCircle(x,y, sec_dot_r, sec_fg);
      last_sec_x = x;
      last_sec_y = y;
    }
}
