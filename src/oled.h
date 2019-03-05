#ifndef __OLED_H__
#define __OLED_H__

class OLED {
 public:
  void init();
  void welcome();
  void update();
  void setText(char* str);
};

#endif