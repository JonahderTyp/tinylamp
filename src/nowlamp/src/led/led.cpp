#include "led.h"

#include <Arduino.h>

LED::LED(uint8_t pin) : m_pin(pin) {
  // Initialize PWM settings
  // set Pin mode to output
  pinMode(m_pin, OUTPUT);
  // Set the initial state of the LED
  m_state = false;
  // Set the initial brightness level
  m_brightness = 255;
  // Set the initial maximum power level
  m_maxPower = 255;
  // Set the initial callback function
  m_callbackFunction = nullptr;

  // Call the callback function
  LED::change();
}

void LED::setCallback(void (*callback)(int, uint8_t)) {
  // Set callback function
  m_callbackFunction = callback;
}

void LED::setBrightness(uint8_t brightness) {
  // Set brightness level
  LED::m_brightness = brightness;
  // Call the callback function
  LED::change();
}

void LED::setMaxPower(uint8_t maxPower) {
  // Set maximum power level
  LED::m_maxPower = maxPower;
  LED::change();
}

uint8_t LED::calculateBrightness() {
  // Calculate the brightness level based on the maximum power level
  if (!LED::m_state) {
    return 0;
  }

  return LED::m_brightness * LED::m_maxPower / 255;
}

void LED::change() {
  if (LED::m_callbackFunction == nullptr) {
    return;
  }
  // call the callback function
  LED::m_callbackFunction(LED::m_pin, LED::calculateBrightness());
}

void LED::on() {
  // Turn the LED on
  LED::m_state = true;
  LED::change();
}

void LED::off() {
  // Turn the LED off
  LED::m_state = false;
  LED::change();
}

void LED::setState(bool state) {
  // Set the state of the LED
  if (state) {
    LED::on();
  } else {
    LED::off();
  }
}

void LED::toggle() {
  // Toggle the state of the LED
  LED::m_state = !LED::m_state;
  LED::change();
}