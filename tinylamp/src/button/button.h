#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
 public:
  // Constructor to initialize the button pin and hold time thresholds
  Button(int pin, unsigned long longPressThreshold = 500,
         unsigned long extraLongPressThreshold = 2000);

  // Method to update the button state, should be called in the loop
  void handle();

  // Method to check if a short press has been detected
  bool isShortPress();

  // Method to check if a long press has been detected
  bool isLongPress();

  // Method to check if an extra long press has been detected
  bool isExtraLongPress();

 private:
  // Pin number where the button is connected
  int buttonPin;
  // Time threshold to differentiate between short and long press
  unsigned long longPressThreshold;
  // Time threshold to differentiate between long and extra long press
  unsigned long extraLongPressThreshold;
  // Time when the button state was last changed
  unsigned long lastDebounceTime;
  // Debounce delay time
  unsigned long debounceDelay;
  // Time when the button was pressed
  unsigned long buttonPressTime;

  // Current state of the button
  bool buttonState;
  // Previous state of the button
  bool lastButtonState;
  // Flag for short press detection
  bool shortPressDetected;
  // Flag for long press detection
  bool longPressDetected;
  // Flag for extra long press detection
  bool extraLongPressDetected;
  // Flag to indicate if the long press has been handled
  bool longPressHandled;
  // Flag to indicate if the extra long press has been handled
  bool extraLongPressHandled;
};

#endif  // BUTTON_H
