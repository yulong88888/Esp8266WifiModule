#ifndef __NTP_H__
#define __NTP_H__

#include "TimeLib.h"

class NTP {
 public:
  time_t getTime();

 private:
  void sendNTPpacket();
};

#endif
