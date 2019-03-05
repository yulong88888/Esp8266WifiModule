// #include <ESP8266WiFi.h>
// #include "Adafruit_MQTT.h"
// #include "Adafruit_MQTT_Client.h"

// #define WLAN_SSID "imaker"
// #define WLAN_PASS "Wlgf@001"

// #define AIO_SERVER "www.lengmang.net"
// #define AIO_SERVERPORT 1883
// #define AIO_USERNAME ""
// #define AIO_KEY ""

// WiFiClient client;

// Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME,
//                           AIO_USERNAME, AIO_KEY);

// Adafruit_MQTT_Subscribe subscribeTask =
//     Adafruit_MQTT_Subscribe(&mqtt, "/WifiModule/publish");

// Adafruit_MQTT_Publish publishTask =
//     Adafruit_MQTT_Publish(&mqtt, "/WifiModule/subscribe");

// void onoffcallback(char *data, uint16_t len) {
//   Serial.print("Hey we're in a onoff callback, the button value is: ");
//   Serial.println(data);
//   publishTask.publish("ok");
// }

// void MQTT_connect();

// void setup() {
//   Serial.begin(115200);
//   delay(10);

//   Serial.println(F("Adafruit MQTT demo"));

//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(WLAN_SSID);

//   WiFi.begin(WLAN_SSID, WLAN_PASS);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println();

//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());

//   subscribeTask.setCallback(onoffcallback);

//   mqtt.subscribe(&subscribeTask);
// }

// uint32_t x = 0;

// void loop() {
//   MQTT_connect();

//   mqtt.processPackets(1000);

//   if (!mqtt.ping()) {
//     mqtt.disconnect();
//   }
// }

// void MQTT_connect() {
//   int8_t ret;

//   if (mqtt.connected()) {
//     return;
//   }

//   Serial.print("Connecting to MQTT... ");

//   uint8_t retries = 3;
//   while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
//     Serial.println(mqtt.connectErrorString(ret));
//     Serial.println("Retrying MQTT connection in 10 seconds...");
//     mqtt.disconnect();
//     delay(10000);  // wait 10 seconds
//     retries--;
//     if (retries == 0) {
//       // basically die and wait for WDT to reset me
//       while (1)
//         ;
//     }
//   }
//   Serial.println("MQTT Connected!");
// }
