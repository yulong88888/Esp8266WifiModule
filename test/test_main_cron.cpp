// #include "Arduino.h"
// #include "string.h"

// String *getData(String str);

// void split(char *src, const char *separator, char **dest, int *num);

// void setup() { Serial.begin(115200); }

// void loop() {
//   // String str = "0-1 . 1/2 . 2. 3. 4.5";
//   // str.trim();
//   // String *fuck = getData(str);
//   // Serial.printf("%s\n", fuck[0].c_str());
//   // Serial.printf("%s\n", fuck[1].c_str());
//   // Serial.printf("%s\n", fuck[2].c_str());

//   char buf[] = "name#sex#age#class#id#";
//   //用来接收返回数据的数组。这里的数组元素只要设置的比分割后的子字符串个数大就好了。
//   char *revbuf[8] = {0};

//   //分割后子字符串的个数
//   int num = 0;

//   split(buf, "#", revbuf, &num);

//   int i = 0;
//   for (i = 0; i < num; i++) {
//     Serial.printf("%s\n", revbuf[i]);
//   }

//   delay(2000);
// }

// void split(char *src, const char *separator, char **dest, int *num) {
//   char *pNext;
//   int count = 0;
//   if (src == NULL || strlen(src) == 0) return;
//   if (separator == NULL || strlen(separator) == 0) return;
//   pNext = strtok(src, separator);
//   while (pNext != NULL) {
//     *dest++ = pNext;
//     ++count;
//     pNext = strtok(NULL, separator);
//   }
//   *num = count;
// }

// bool match(String s, time_t timeValue) {
//   if (s.equals("*")) {
//     return true;
//   }
//   if (s.indexOf("-")) {
//     time_t first = s.substring(0, s.indexOf("-")).toInt();
//     time_t second = s.substring(s.indexOf("-")).toInt();
//     if (first > second) {
//       time_t temp = first;
//       first = second;
//       second = temp;
//     }
//     if (timeValue >= first && timeValue <= second) {
//       return true;
//     } else {
//       return false;
//     }
//   }
//   if (s.indexOf("/")) {
//     time_t first = s.substring(0, s.indexOf("-")).toInt();
//     time_t second = s.substring(s.indexOf("-")).toInt();
//     if (first > second) {
//       time_t temp = first;
//       first = second;
//       second = temp;
//     }
//     int result = int((timeValue - first) / second % 2);
//     if (result <= 0) {
//       return false;
//     }
//     if (result) {
//       return true;
//     } else {
//       return false;
//     }
//   }
//   return false;
// }

// // char **getData(char *str) {
// //   int n = 0;
// //   char **temp;
// //   char *p = strtok(str, ".");
// //   while (p) {
// //     memcmp((char *)temp[n][0], p, strlen(p));
// //     p = strtok(NULL, ".");
// //     n++;
// //   }
// //   return temp;
// // }

// /*
// String *splitData(String str) {
//   int n = 0;
//   char *temp = (char *)str.c_str();
//   for (int i = 0; temp[i] != '\0'; i++) {
//     if (temp[i] == '.') {
//       n++;
//     }
//   }
//   String *strArr = new String[n + 1];
//   n = 0;
//   char *p = strtok(temp, ".");
//   while (p) {
//     strArr[n] = String(p);
//     p = strtok(NULL, ".");
//     n++;
//   }
//   return strArr;
// }
// */
