/*
 * Cron.h
 *
 *  Created on: 7 Jun 2012
 *      Author: fletcherb
 */

#ifndef __CRON_H__
#define __CRON_H__

#include "Arduino.h"

class Cron {
 public:
  virtual ~Cron();
  void printTime();
  void initTime(time_t time);
  boolean matchCron(String str);

 private:
  bool match(String s, time_t timeValue);
  void split(char *src, const char *separator, char **dest, int *num);
};

#endif
