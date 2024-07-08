#include <cstdint>

#ifndef _led_h
#define _led_h

class LED {
public:
  LED(uint8_t pin);

  void setBrightness(uint8_t brightness);

  void setMaxPower(uint8_t maxPower);

  // Set callback function for the LED class to call when the LED changes
  // state or brightness level
  void setCallback(void (*callback)(int, uint8_t));

  void on();
  void off();
  void setState(bool state);
  void toggle();

private:
  uint8_t calculateBrightness();
  void change();

  uint8_t m_pin;
  uint8_t m_maxPower;
  uint8_t m_brightness;
  bool m_state;
  void (*m_callbackFunction)(int, uint8_t);
};

#endif