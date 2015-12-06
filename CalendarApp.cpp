#include "CalendarApp.h"

void CalendarApp::run() {
  if (pad.down(LEFT_PAD)) {
    month--;
    if (month < 1) {
      month = 12;
      year--;
    }
    draw();
  }
  if (pad.down(RIGHT_PAD)) {
    month++;
    if (month > 12) {
      month = 1;
      year++;
    }
    draw();
  }
};

void CalendarApp::draw() {
  clearScreen();
  coord_t rowHeight = screen.clipHeight() / 9 + 1; // round up otherwise there's too much space at the bottom
  coord_t colWidth = screen.clipWidth() / 8;
  coord_t xStart = screen.clipLeft() + colWidth;
  coord_t xEnd = xStart + 6*colWidth;
  coord_t yStart = screen.clipTop() + rowHeight;


  // draw the month/year
  coord_t x = screen.clipMidWidth();
  coord_t y = yStart;

  char s[100];
  sprintf(s, "%s %d", clock.monthString(month), year);
  screen.setFont(Arial_14_Bold);
  screen.setTextColor(screen.red);
  screen.setCursor( x - screen.measureTextWidth(s)/2, y - screen.measureTextHeight(s)/2);
  screen.drawText(s);

  // draw the day header
  y += rowHeight;
  x = xStart;
  const char* dayAbbrev[] = { "", "Su", "M", "Tu", "W", "Th", "F", "Sa" };
  screen.setFont(Arial_14);
  screen.setTextColor(screen.green);
  for (int i = 1; i <= 7; i++) {
    screen.setCursor( x - screen.measureTextWidth(dayAbbrev[i])/2, y - screen.measureTextHeight(dayAbbrev[i])/2);
    screen.drawText(dayAbbrev[i]);
    x += colWidth;
  }

  Time t;
  t.set(year,month,1);

  y += rowHeight;
  x = xStart + (t.weekday()-1)*colWidth;
  screen.setTextColor(screen.blue);

  for (int i = 1; i <= t.daysInMonth(); i++) {
    sprintf(s, "%d", i);

    if (year == clock.year() && month == clock.month() && i == clock.day() ) {
      screen.fillCircle(x,y,(rowHeight+1)/2, screen.yellow);
    }

    screen.setCursor( x - screen.measureTextWidth(s)/2, y - screen.measureTextHeight(s)/2);
    screen.drawText(s);

    x += colWidth;
    if (x > xEnd) {
      x = xStart;
      y+= rowHeight;
    }
  }

  y+= rowHeight;
};

