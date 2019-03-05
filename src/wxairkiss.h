#ifndef __WXAIRKISS_H__
#define __WXAIRKISS_H__

class WxAirkiss {
 private:
  typedef void (*ResetCallback)();
  ResetCallback resetCallback = nullptr;

 public:
  WxAirkiss();
  void init();
  bool isConnect(ResetCallback resetCallback);
  void start();
  void clear();

  char* getMacAddress();

  void discovery();
};
#endif
