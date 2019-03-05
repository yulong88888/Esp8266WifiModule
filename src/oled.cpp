#include "oled.h"
#include "OLEDDisplayUi.h"
#include "SSD1306Wire.h"
#include "Wire.h"

#include "images.h"

SSD1306Wire display(0x3c, 2, 0);

OLEDDisplayUi ui(&display);

void welcome(OLEDDisplay* display, OLEDDisplayUiState* state, int16_t x,
             int16_t y) {
  display->drawXbm(x + 34, y + 14, WiFi_Logo_width, WiFi_Logo_height,
                   WiFi_Logo_bits);
}

FrameCallback frames[] = {welcome};

void OLED::init() {
  ui.init();
  //反转180度
  display.flipScreenVertically();
}

void OLED::welcome() {
  ui.setFrames(frames, 0);
  ui.update();
}

void OLED::update() { ui.update(); }

void OLED::setText(char* str) {
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, str);
  display.display();
}
