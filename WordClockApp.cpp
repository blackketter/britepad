#include "WordClockApp.h"
#include "Debug.h"

const char* minutes[] = {
"","one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",
"twenty", "twenty-one", "twenty-two", "twenty-three", "twenty-four", "twenty-five", "twenty-six", "twenty-seven", "twenty-eight", "twenty-nine",
"thirty", "thirty-one", "thirty-two", "thirty-three", "thirty-four", "thirty-five", "thirty-six", "thirty-seven", "thirty-eight", "thirty-nine",
"forty", "forty-one", "forty-two", "forty-three", "forty-four", "forty-five", "forty-six", "forty-seven", "forty-eight", "forty-nine",
"fifty", "fifty-one", "fifty-two", "fifty-three", "fifty-four", "fifty-five", "fifty-six", "fifty-seven", "fifty-eight", "fifty-nine" };

const char* days[] = {
"", "first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "ninth", "tenth",
"eleventh", "twelfth", "thirteenth", "forteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "ninteenth", "twentieth",
"twenty-first", "twenty-second", "twenty-third", "twenty-fourth", "twenty-fifth", "twenty-sixth", "twenty-seventh", "twenty-eighth", "twenty-ninth",
"thirtieth", "thirty-first" };

const char* hours[] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "noon", "midnight" };

void WordClockApp::update() {
    clearScreen();
    char words[500];

    screen.setFont(Arial_20_Bold);

    screen.setTextColor(currentColor++, bgColor());

// It's about five twenty-six in the afternoon on Friday, January twenty-sixth, two-thousand and fifteen
    const char* wordsformat = "It's %s%s %s%s on %s, %s %s in the year two-thousand and %s.";
    const char* wordsformatpast = "It's %s%s minute%s past %s%s on %s, %s %s in the year two-thousand and %s.";
    const char* wordsformatto = "It's %s%s minute%s to %s%s on %s, %s %s in the year two-thousand and %s.";

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
    switch (now.hour()) {
      case 0:
      case 12:
      case 24:
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
        timeofday = " in the morning";
        break;
      case 13:
      case 14:
      case 15:
      case 16:
        timeofday = " in the afternoon";
        break;
      case 17:
      case 18:
      case 19:
      case 20:
        timeofday = " in the evening";
        break;
      default:
        timeofday = " at night";
        break;
      }

    int i = 0;

    coord_t left = screen.clipLeft() + 8;
    coord_t right = screen.clipRight() - 8;
    screen.setCursor(left, screen.clipTop() + 12);
    int hour = now.hour() == 0 ? 13 : now.hourFormat12();

    if (now.minute() < 15 && now.minute() > 0) {
      sprintf(words, wordsformatpast, about, minutes[now.minute()], (now.minute() == 1) ? "" : "s", hours[hour], timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else if (now.minute() > 44) {
      int tohour = now.hourFormat12() + 1;
      if (tohour > 12) {
        tohour = 1;
      }
      if (now.hour() == 23) {
        tohour = 13;
      }
      sprintf(words, wordsformatto, about, minutes[60-now.minute()], (now.minute() == 59) ? "" : "s", hours[tohour], timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    } else {
      sprintf(words, wordsformat, about, hours[hour], minutes[now.minute()], timeofday, now.weekdayString(), now.monthString(), days[now.day()], minutes[now.year() % 2000]);
    }

    while (words[i]) {
      coord_t x = screen.getCursorX();
      char curWord[20];
      char* curPos = words+i;

      char* nextSpace = strchr(curPos, ' ');
      char* nextDash = strchr(curPos, '-');

      char* next = nextSpace;
      if (nextSpace && nextDash) {
        next = min(nextSpace,nextDash);
      } else if (nextDash) {
        next = nextDash;
      }

      int wordLen = 0;

      if (next == nullptr) {
        wordLen = strlen(curPos);
      } else {
        wordLen = next - curPos + 1;
      }

      strncpy(curWord, curPos, wordLen);
      curWord[wordLen] = 0;


      if (x+screen.measureTextWidth(curWord) > right) {
        screen.setCursor(left, screen.getCursorY()+screen.measureTextHeight(curWord)+screen.fontLineSpacing());
      }
      screen.drawText(curWord);
      i+=wordLen;
    }
}

