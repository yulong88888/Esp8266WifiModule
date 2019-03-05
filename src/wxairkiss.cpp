#include "wxairkiss.h"
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#include "airkiss.h"
#include "config.h"

//四维拓智公众账号
String DEVICE_TYPE = "gh_5e8661030bb8";
// longge8849测试号
// String DEVICE_TYPE = "gh_18802359d5ab";
String DEVICE_ID = "4Dclass_WifiModule_";

WxAirkiss::WxAirkiss() {
  //初始化设备ID
  DEVICE_ID = DEVICE_ID + WiFi.softAPmacAddress().c_str();
}

/**
   初始化
*/
void WxAirkiss::init() { WiFi.begin(); }

/**
   检查连接状态
*/
bool WxAirkiss::isConnect(ResetCallback resetCallback) {
  this->resetCallback = resetCallback;
  unsigned long cache = millis();
  String ssid = WiFi.SSID().c_str();
  while (1) {
    if (ssid.length() == 0) {
      return false;
    }
    if (millis() - cache > 500) {
      cache = millis();
      int state = WiFi.status();
      if (state == WL_CONNECTED) {
        return true;
      }
    }
    this->resetCallback();
    //防止系统触发看门狗
    delay(50);
  }
}

/**
   启动airkiss
*/
void WxAirkiss::start() {
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  while (1) {
    if (WiFi.smartConfigDone()) {
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    delay(1000);  // 这个地方一定要加延时，否则极易崩溃重启
  }
}

/**
   删除wifi连接记录
*/
void WxAirkiss::clear() {
  ESP.eraseConfig();
  delay(100);
  ESP.reset();
  delay(100);
  ESP.restart();
}

/**
   获取mac地址
*/
char *WxAirkiss::getMacAddress() { return (char *)DEVICE_ID.c_str(); }

//==================================================================================================
WiFiUDP udp;

char udpPacket[255];
//用于缓存回包的数据缓冲区，也可以为局部变量
uint8_t lan_buf[255];
uint16_t lan_buf_len;

const airkiss_config_t airkissConf = {(airkiss_memset_fn)&memset,
                                      (airkiss_memcpy_fn)&memcpy,
                                      (airkiss_memcmp_fn)&memcmp, 0};

/**
   开启微信局域网发现
*/
void WxAirkiss::discovery() {
  udp.begin(DEFAULT_LAN_PORT);
  unsigned long beginTime = 0;
  while (1) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      int len = udp.read(udpPacket, 255);
      if (len > 0) {
        udpPacket[len] = 0;
      }
      int ret = airkiss_lan_recv(udpPacket, len, &airkissConf);
      int packret;
      switch (ret) {
        //接收到发现设备请求数据包
        case AIRKISS_LAN_SSDP_REQ:
          lan_buf_len = sizeof(lan_buf);
          //打包数据
          packret = airkiss_lan_pack(AIRKISS_LAN_SSDP_RESP_CMD,
                                     (char *)DEVICE_TYPE.c_str(),
                                     (char *)DEVICE_ID.c_str(), 0, 0, lan_buf,
                                     &lan_buf_len, &airkissConf);
          if (packret != AIRKISS_LAN_PAKE_READY) {
            return;
          }
          udp.beginPacket(udp.remoteIP(), udp.remotePort());
          Serial.println(udp.write(lan_buf, sizeof(lan_buf)));
          udp.endPacket();
          //刷新开始UDP时间
          beginTime = millis();
          break;
        default:
          break;
      }
    }
    //判断是否完成局域网发现
    if (beginTime != 0 && millis() - beginTime > 5000) {
      beginTime = 0;
      return;
    }
  }
}
