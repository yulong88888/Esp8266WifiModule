#include "Cron.h"
#include <TimeLib.h>

Cron::~Cron() {}

boolean Cron::matchCron(String cronString) {
  boolean secMatch, minMatch, hourMatch, dayMatch, weekMatch, monMatch,
      yearMatch;

  cronString.trim();

  char *cronStrs[7] = {0};
  int num = 0;

  split((char *)cronString.c_str(), ".", cronStrs, &num);

  // for (int i = 0; i < num; i++) {
  //   printf("%s\n", cronStrs[i]);
  // }

  secMatch = match(cronStrs[0], second());
  minMatch = match(cronStrs[1], minute());
  hourMatch = match(cronStrs[2], hour());
  dayMatch = match(cronStrs[3], day());
  weekMatch = match(cronStrs[4], weekday());
  monMatch = match(cronStrs[5], month());
  yearMatch = match(cronStrs[6], year());

  // Serial.print(secMatch);
  // Serial.print(minMatch);
  // Serial.print(hourMatch);
  // Serial.print(dayMatch);
  // Serial.print(weekMatch);
  // Serial.print(monMatch);
  // Serial.print(yearMatch);
  // Serial.println();

  if (secMatch && minMatch && hourMatch && dayMatch && weekMatch && monMatch &&
      yearMatch) {
    return true;
  } else {
    return false;
  }
}

void Cron::printTime() {
  Serial.print(year(), DEC);
  Serial.print('/');
  Serial.print(month(), DEC);
  Serial.print('/');
  Serial.print(day(), DEC);
  Serial.print(' ');
  Serial.print(hour(), DEC);
  Serial.print(':');
  Serial.print(minute(), DEC);
  Serial.print(':');
  Serial.print(second(), DEC);
  Serial.println();
}

void Cron::initTime(time_t time) { setTime(time); }

/**
 * 新加入的匹配
 */
bool Cron::match(String s, time_t timeValue) {
  if (s.equals("*")) {
    return true;
  }
  if (s.indexOf("-") > 0) {
    time_t first = s.substring(0, s.indexOf("-")).toInt();
    time_t second = s.substring(s.indexOf("-") + 1).toInt();
    if (first > second) {
      time_t temp = first;
      first = second;
      second = temp;
    }
    if (timeValue >= first && timeValue <= second) {
      return true;
    } else {
      return false;
    }
  }
  if (s.indexOf("/") > 0) {
    int first = s.substring(0, s.indexOf("/")).toInt();
    int second = s.substring(s.indexOf("/") + 1).toInt();
    Serial.println(first);
    Serial.println(second);
    // if (first > second) {
    //   int temp = first;
    //   first = second;
    //   second = temp;
    // }
    if (first == 0) {
      first = 1;
    }
    if (second == 0) {
      return true;
    }
    int result = int((timeValue - first) / second % 2);
    if (result <= 0) {
      return false;
    }
    if (result) {
      return true;
    } else {
      return false;
    }
  }
  if (s.indexOf(",") > 0) {
    char *weekStrs[7] = {0};
    int num = 0;
    split((char *)s.c_str(), ",", weekStrs, &num);
    bool result = false;
    for (int i = 0; i < num; i++) {
      String tempStr = weekStrs[i];
      if (timeValue == tempStr.toInt()) {
        result = true;
      }
    }
    return result;
  }
  time_t temp = s.toInt();
  if (temp == timeValue) {
    return true;
  }
  return false;
}

/**
 * 新加入Cron表达式解析
 */
void Cron::split(char *src, const char *separator, char **dest, int *num) {
  char *pNext;
  int count = 0;
  if (src == NULL || strlen(src) == 0) return;
  if (separator == NULL || strlen(separator) == 0) return;
  pNext = strtok(src, separator);
  while (pNext != NULL) {
    *dest++ = pNext;
    ++count;
    pNext = strtok(NULL, separator);
  }
  *num = count;
}
