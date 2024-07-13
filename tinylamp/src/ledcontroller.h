#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#include "valuewheel.h"

class LedController {
 public:
  LedController(uint8_t Pin) : strip(10, Pin, NEO_GRB + NEO_KHZ800) {
    strip.begin();
    strip.setBrightness(255);
    strip.show();
  }

  template <typename T1, typename T2>
  void menu(ValueWheel<T1>& _menuWheel, ValueWheel<T2>& _setting) {
    unsigned int color = numberWheel.getViaIndex(_menuWheel.getIndex());
    size_t value = (uint8_t)_setting.getIndex();

    strip.setPixelColor(0, color);

    for (size_t i = 0; i < 8; i++) {
      if (value & (1 << i)) {
        strip.setPixelColor(i + 1, 0xFFFF00);
      } else {
        strip.setPixelColor(i + 1, 0xFF0000);
      }
    }
    strip.show();
  }

  void increaseColor() {
    if (brightnessWheel.getIndex() == 0) {
      brightnessWheel.increaseIndex();
    } else {
      colorWheel.increaseIndex();
    }
    this->updateColor();
  }

  void increaseBrightness() {
    brightnessWheel.increaseIndex();
    this->updateColor();
  }

  // void decreaseColor() {
  //   colorWheel.decreaseIndex();
  //   if (brightnessWheel.getIndex() == 0) {
  //     brightnessWheel.increaseIndex();
  //   }
  //   this->updateColor();
  // }

  // void decreaseBrightness() {
  //   brightnessWheel.decreaseIndex();
  //   this->updateColor();
  // }

  void turnOff() {
    brightnessWheel.setIndex(0);
    this->updateColor();
  }

  uint8_t getColor() {
    return colorWheel.getIndex();
  }
  uint8_t getBrightness() {
    return brightnessWheel.getIndex();
  }

  // void strobe(bool activate, uint8_t colorIndex = 6,
  //             uint8_t brightness = NUMBRIGHTNESS - 1) {
  //   this->strobeColorIndex = colorIndex;
  //   this->strobeBrightness = brightness;
  //   this->strobeActive = activate;
  //   if (!strobeActive) {
  //     this->updateColor();
  //   }
  // }

  void loop() {
    // if (strobeActive) {
    //   static unsigned long lastTime = 0;
    //   unsigned long currentTime = millis();
    //   if (currentTime - lastTime > 100) {  // Change strobe speed as needed
    //     if (strobeState) {
    //       writeColor(0, this->strobeBrightness);
    //     } else {
    //       writeColor(strobeColorIndex, this->strobeBrightness);
    //     }
    //     strobeState = !strobeState;
    //     lastTime = currentTime;
    //   }
    // }
  }

  void clear() {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
  }

 private:
  Adafruit_NeoPixel strip;
  // bool strobeActive;
  // bool strobeState = false;
  // uint8_t strobeColorIndex = 6;    // Default to white
  // uint8_t strobeBrightness = 255;  // Default to maximum brightness
  // uint8_t colorIndex = 0;                       // Default to black
  // uint8_t brightnessIndex = NUMBRIGHTNESS - 1;  // Default to maximum
  // brightness

  ValueWheel<unsigned int> colorWheel = ValueWheel<unsigned int>({
      strip.Color(255, 0, 0),     // Red
      strip.Color(255, 255, 0),   // Yellow
      strip.Color(0, 255, 0),     // Green
      strip.Color(0, 255, 255),   // Cyan
      strip.Color(0, 0, 255),     // Blue
      strip.Color(255, 0, 255),   // Magenta
      strip.Color(255, 255, 255)  // White
  });                             // 0-6 are the color indexes

  ValueWheel<uint8_t> brightnessWheel = ValueWheel<uint8_t>({
      0,
      1,
      8,
      64,
      255,
  });  // 0-7 are the brightness indexes

  ValueWheel<unsigned int> numberWheel = ValueWheel<unsigned int>(
      {0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF});

  uint32_t calculateColor(uint8_t colorIndex, uint8_t brightnessIndex) {
    uint32_t color = colorWheel.getViaIndex(colorIndex);
    uint8_t brightness = brightnessWheel.getViaIndex(brightnessIndex);

    uint8_t r = (uint8_t)((color >> 16) & 0xFF);
    uint8_t g = (uint8_t)((color >> 8) & 0xFF);
    uint8_t b = (uint8_t)(color & 0xFF);

    r = (r * brightness) / 255;
    g = (g * brightness) / 255;
    b = (b * brightness) / 255;

    return strip.Color(r, g, b);
  }

  void writeColor(uint8_t colorIndex, uint8_t brightnessIndex) {
    uint32_t actualColor = calculateColor(colorIndex, brightnessIndex);
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, actualColor);
    }
    strip.show();
  }

  void updateColor() {
    writeColor(colorWheel.getIndex(), brightnessWheel.getIndex());
  }
};

#endif  // LEDCONTROLLER_H