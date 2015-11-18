#include "WordClockApp.h"
#include "Debug.h"

static const char* minutes[] = {
"","one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",
"twenty", "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five", "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",
"thirty", "thirty-one", "thirty-two", "thirty-three", "thirty-four", "thirty-five", "thirty-six", "thirty-seven", "thirty-eight", "thirty-nine",
"forty", "forty-one", "forty-two", "forty-three", "forty-four", "forty-five", "forty-six", "forty-seven", "forty-eight", "forty-nine",
"fifty", "fifty-one", "fifty-two", "fifty-three", "fifty-four", "fifty-five", "fifty-six", "fifty-seven", "fifty-eight", "fifty-nine" };

static const char* days[] = {
"", "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth",
"eleventh", "twelfth", "thirteenth", "forteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "ninteenth", "twentieth",
"twenty-first", "twenty-second", "twenty-third", "twenty-fourth", "twenty-fifth", "twenty-sixth", "twenty-seventh", "twenty-eighth", "twenty-ninth",
"thirtieth", "thirty-first" };

static const char* hours[] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve" };

void WordClockApp::run() {
  millis_t mt = clock.millis();

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
    now.set(clock.now());

    if (now.second() < 5) {
      about = "";
    } else if (now.second() > 55) {
      about = "almost ";
      now.set(now.get() + 60);
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
        hour = hours[hournum];
        timeofday = "in the morning ";
        break;
      case 13:
      case 14:
      case 15:
      case 16:
        hour = hours[hournum-12];
        timeofday = "in the afternoon ";
        break;
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
        hour = hours[hournum-12];
        timeofday = "in the evening ";
        break;
      default: // 22 and up
        hour = hours[hournum-12];
        timeofday = "at night ";
        break;
      }

    screen.setFont(Arial_20_Bold);
    screen.setTextColor(currentColor++, bgColor());
    screen.setCursor(screen.clipLeft() + 8, screen.clipTop() + 12);

    if (now.minute() == 0) {
      static const char* wordsonthehourformat = "It's %s%s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsonthehourformat, about, hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (now.minute() == 30) {
      static const char* wordsformathalfpast = "It's %shalf past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (now.minute() == 15) {
      static const char* wordsformathalfpast = "It's %squarter past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (now.minute() == 45) {
      static const char* wordsformathalfpast = "It's %squarter to %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformathalfpast, about, hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (afterhour) {
      static const char* wordsformatpast = "It's %s%s minute%s past %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformatpast, about, minutes[now.minute()], (now.minute() == 1) ? "" : "s", hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (beforehour) {
      static const char* wordsformatto = "It's %s%s minute%s to %s %son %s, %s %s, two-thousand and %s.";
      sprintf(words, wordsformatto, about, minutes[60-now.minute()], (now.minute() == 59) ? "" : "s", hour, timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else {
      static const char* wordsformat = "It's %s%s %s %son %s, %s %s, two-thousand and %s.";
      if (hournum == 12) {
         hour = "twelve";
         timeofday = "in the afternoon ";
      } else if (hournum == 0) {
        hour = "twelve";
        timeofday = "in the morning ";
      }
      sprintf(words, wordsformat, about, hour, minutes[now.minute()], timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    }

    screen.drawText(words, screen.lineBreakChars);
}

