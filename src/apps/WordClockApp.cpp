#include "WordClockApp.h"
#include "Debug.h"
#include "Strings.h"

WordClockApp theWordClockApp;

void WordClockApp::run() {
  millis_t mt = Uptime::millis();

  if (lastUpdate == 0 || mt - lastUpdate > updateInterval()) {
    update();
    lastUpdate = mt;
  } else if (0){
    screen.setScroll(scrollPos+=1);
    if (scrollPos >= screen.width()) {
      scrollPos = 0;
    }
  }
}

void WordClockApp::end() {
  screen.setScroll(0);
  ClockApp::end();
}

void WordClockApp::update() {
    clearScreen();
    char words[500];
    const char* hour;

    const char* about;
    Time now;
    now.setSeconds(clock.now());

    if (now.second() < 5) {
      about = "";
    } else if (now.second() > 55) {
      about = "almost ";
      now.setSeconds(now.getSeconds() + 60);
    } else {
      about = "about ";
    }

    const char* timeofday;
    int hournum = now.hour();

    bool beforehour = now.minute() > 49 ||  now.minute() == 45;
    if (beforehour) {
      hournum++;
    }

    bool afterhour = now.minute() < 15 && now.minute() > 0;


    switch (hournum) {
      case 0:
        hour = "midnight";
        timeofday = "";
        break;
      case 12:
        hour = "noon";
        timeofday = "";
        break;
      case 24:
        hour = "midnight";
        timeofday = "";
        break;
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
        hour = numberStrings[hournum];
        timeofday = "in the morning ";
        break;
      case 13:
      case 14:
      case 15:
      case 16:
        hour = numberStrings[hournum-12];
        timeofday = "in the afternoon ";
        break;
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
        hour = numberStrings[hournum-12];
        timeofday = "in the evening ";
        break;
      default: // 22 and up
        hour = numberStrings[hournum-12];
        timeofday = "at night ";
        break;
      }

    screen.setFont(Arial_20_Bold);
    screen.setTextColor(currentColor++, bgColor());
    screen.setCursor(screen.clipLeft() + 8, screen.clipTop() + 12);

    if (now.minute() == 0) {
      static const char* wordsonthehourformat = "It's %s%s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsonthehourformat, about, hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else if (now.minute() == 30) {
      static const char* wordsformathalfpast = "It's %shalf past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else if (now.minute() == 15) {
      static const char* wordsformathalfpast = "It's %squarter past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else if (now.minute() == 45) {
      static const char* wordsformathalfpast = "It's %squarter to %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else if (afterhour) {
      static const char* wordsformatpast = "It's %s%s minute%s past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformatpast, about, numberStrings[now.minute()], (now.minute() == 1) ? "" : "s", hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else if (beforehour) {
      static const char* wordsformatto = "It's %s%s minute%s to %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformatto, about, numberStrings[60-now.minute()], (now.minute() == 59) ? "" : "s", hour, timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    } else {
      static const char* wordsformat = "It's %s%s %s %son %s, %s %s, two-thousand and %s.";
      if (hournum == 12) {
         hour = "twelve";
         timeofday = "in the afternoon ";
      } else if (hournum == 0) {
        hour = "twelve";
        timeofday = "in the morning ";
      }
      sprintf(words, wordsformat, about, hour, numberStrings[now.minute()], timeofday, now.weekdayString(), now.monthString(), ordinalStrings[now.day()], numberStrings[now.year() % 2000]);
    }

    screen.drawText(words, screen.lineBreakChars);
}

