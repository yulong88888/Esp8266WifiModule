#include "Arduino.h"
#include "ArduinoJson.h"
#include "Cron.h"
#include "ESP8266WiFi.h"
#include "MQTT.h"
#include "QList.h"
#include "Time.h"
#include "config.h"
#include "ntp.h"
#include "oled.h"
#include "wxairkiss.h"

OLED oled;
WxAirkiss wxAirkiss;
NTP ntp;

WiFiClient wifiClient;
MQTTClient mqttClient;

// Cron表达式
Cron cron;

struct CommunicationObj {
  bool state = false;
  int mode = -1;
  QList<String> cron;

} communicationObj;

/**
 * 设置输出引脚
 */
void setOutput(bool b) {
  communicationObj.state = b;
  // if (b == digitalRead(OUTPUT_PIN)) {
  //   return;
  // } else {
  digitalWrite(OUTPUT_PIN, b);
  // }
}

//设备的逻辑是反的，方便上位机阅读
String getUrl(bool isSubscribe) {
  String temp1 = PROJECT_TYPE;
  String temp2 = wxAirkiss.getMacAddress();
  String result = "";
  if (isSubscribe) {
    result = "/" + temp1 + "/Subscribe/" + temp2;
  } else {
    result = "/" + temp1 + "/Publish/" + temp2;
  }
  return result;
}

/**
 * MQTT回调函数
 */
void taskCallback(String &topic, String &payload) {
  Serial.println(payload);
  // oled.setText((char *)payload.c_str());
  // StaticJsonBuffer<512> jsonBuffer;
  // JsonObject &root = jsonBuffer.parseObject(payload);

  DynamicJsonDocument root(512);
  deserializeJson(root, payload);

  String id = root["id"];
  int mode = root["mode"];
  bool state = root["state"];
  // JsonArray &array = root["cron"];
  JsonArray array = root["cron"];
  //手动模式
  if (mode == -1) {
    setOutput(state);
  }
  communicationObj.mode = mode;
  communicationObj.cron.clear();
  //设置cron表达式
  for (int i = 0; i < array.size(); i++) {
    String temp = root["cron"][i];
    Serial.println(temp);
    communicationObj.cron.push_back(temp);
  }
  String jsonStr = "{\"ok\":\"%ID\"}";
  jsonStr.replace("%ID", id);
  mqttClient.publish((char *)getUrl(true).c_str(), jsonStr);
}

/**
 * 系统初始化
 */
void init_sys() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(10);
  digitalWrite(LED_PIN, HIGH);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(MANUAL_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  oled.init();
  wxAirkiss.init();
}

/**
 * MQTT连接
 */
void conn_mqtt() {
  oled.setText((char *)"conn server");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  //客户端id 连接的用户名 连接的密码
  while (!mqttClient.connect("", "", "")) {
    delay(1000);
  }

  mqttClient.subscribe((char *)getUrl(false).c_str());
}

//时间格式
String format(String str) {
  if (str.toInt() < 10) {
    return "0" + str;
  } else {
    return str;
  }
}

/**
 * 清空WiFi信息(重新配网)
 */
long btnManualPressTime = 0;
long btnResetPressTime = 0;

void manual() {
  if (digitalRead(MANUAL_PIN)) {
    if (btnManualPressTime != 0) {
      if (millis() - btnManualPressTime < 1000 && btnManualPressTime > 0) {
        oled.setText((char *)"manual");
        setOutput(!communicationObj.state);
      }
      btnManualPressTime = 0;
    }
  } else {
    delay(10);
    if (btnManualPressTime < 0) {
      return;
    }
    if (btnManualPressTime == 0) {
      btnManualPressTime = millis();
    }
    if (millis() - btnManualPressTime > 3000) {
      oled.setText((char *)"fuck");
      btnManualPressTime = -1;
    }
  }
}

void reset() {
  if (digitalRead(RESET_PIN)) {
    if (btnResetPressTime != 0) {
      btnResetPressTime = 0;
    }
  } else {
    if (btnResetPressTime == 0) {
      btnResetPressTime = millis();
    }
    if (millis() - btnResetPressTime > 3000) {
      oled.setText((char *)"reset");
      wxAirkiss.clear();
    }
  }
}

void setup() {
  init_sys();
  oled.welcome();
  delay(1000);
  //网络连接检测
  if (!wxAirkiss.isConnect(reset)) {
    oled.setText((char *)"wait airkiss");
    wxAirkiss.start();
    oled.setText((char *)"wait bind");
    wxAirkiss.discovery();
  }
  //初始化Cron时间
  cron.initTime(ntp.getTime());
  // MQTT初始化
  mqttClient.begin(MQTT_SERVER, MQTT_SERVER_PORT, wifiClient);
  mqttClient.onMessage(taskCallback);
  conn_mqtt();
  oled.setText((char *)"run");
}

time_t showTime = 0;
unsigned long mqttLoopTime = 0;
unsigned long sendCommunicationObjTime = 0;

void loop() {
  // fixes some issues with WiFi stability
  if (millis() - mqttLoopTime > 10) {
    mqttLoopTime = millis();
    mqttClient.loop();
  }

  if (!mqttClient.connected()) {
    conn_mqtt();
  }

  switch (communicationObj.mode) {
    //仅一次模式
    case 0:
      for (int i = 0; i < communicationObj.cron.length(); i++) {
        if (cron.matchCron(communicationObj.cron.get(i))) {
          if (i % 2 == 0) {
            setOutput(true);
          } else {
            setOutput(false);
          }
        }
      }
      break;
    //循环模式
    case 1:
      for (int i = 0; i < communicationObj.cron.length(); i++) {
        if (cron.matchCron(communicationObj.cron.get(i))) {
          setOutput(true);
        } else {
          setOutput(false);
        }
      }
      break;
    //星期模式
    case 2:
      for (int i = 0; i < communicationObj.cron.length(); i++) {
        if (cron.matchCron(communicationObj.cron.get(i))) {
          if (i % 2 == 0) {
            setOutput(true);
          } else {
            setOutput(false);
          }
        }
      }
      break;
    default:
      break;
  }

  if (showTime != now()) {
    showTime = now();
    String h = format(String(hour()));
    String m = format(String(minute()));
    String s = format(String(second()));
    String temp = h + ":" + m + ":" + s;
    oled.setText((char *)temp.c_str());
  }

  if (millis() - sendCommunicationObjTime > 1000) {
    String jsonStr = "{\"state\":%S,\"mode\":%M,\"cron\":[%C]}";
    if (communicationObj.state) {
      jsonStr.replace("%S", "true");
    } else {
      jsonStr.replace("%S", "false");
    }
    jsonStr.replace("%M", String(communicationObj.mode));
    String temp = "";
    for (int i = 0; i < communicationObj.cron.length(); i++) {
      temp += "\"" + communicationObj.cron.get(i) + "\"";
      if (i != communicationObj.cron.length() - 1) {
        temp += ",";
      }
    }
    jsonStr.replace("%C", temp);
    // Serial.println(jsonStr);
    mqttClient.publish((char *)getUrl(true).c_str(), jsonStr);
    sendCommunicationObjTime = millis();
  }

  reset();
  manual();
}
