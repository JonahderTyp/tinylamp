#include "button.h"

#include <Arduino.h>

Button::Button(int pin, unsigned long longPressThreshold,
               unsigned long extraLongPressThreshold)
    : buttonPin(pin),
      longPressThreshold(longPressThreshold),
      extraLongPressThreshold(extraLongPressThreshold),
      lastDebounceTime(0),
      debounceDelay(50),
      buttonPressTime(0),
      buttonState(LOW),
      lastButtonState(LOW),
      shortPressDetected(false),
      longPressDetected(false),
      extraLongPressDetected(false),
      longPressHandled(false),
      extraLongPressHandled(false) {
  pinMode(buttonPin, INPUT_PULLUP);
}

// Method to update the button state
void Button::handle() {
  int reading = !digitalRead(buttonPin);

  // Check for state change and handle debouncing
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Detect button press
      if (buttonState == HIGH) {
        buttonPressTime = millis();
        shortPressDetected = false;
        longPressDetected = false;
        extraLongPressDetected = false;
        longPressHandled = false;
        extraLongPressHandled = false;
      }
      // Detect button release
      else {
        if (!extraLongPressHandled &&
            (millis() - buttonPressTime) >= extraLongPressThreshold) {
          extraLongPressDetected = true;
        } else if (!longPressHandled &&
                   (millis() - buttonPressTime) >= longPressThreshold) {
          longPressDetected = true;
        } else if ((millis() - buttonPressTime) < longPressThreshold) {
          shortPressDetected = true;
        }
        longPressHandled = false;  // Reset long press handled flag on release
        extraLongPressHandled =
            false;  // Reset extra long press handled flag on release
      }
    }

    // Check for long press while button is held down
    if (buttonState == HIGH) {
      if ((millis() - buttonPressTime) >= extraLongPressThreshold &&
          !extraLongPressHandled) {
        extraLongPressDetected = true;
        extraLongPressHandled =
            true;  // Ensure extra long press is handled only once per press
      } else if ((millis() - buttonPressTime) >= longPressThreshold &&
                 !longPressHandled) {
        longPressDetected = true;
        longPressHandled =
            true;  // Ensure long press is handled only once per press
      }
    }
  }

  lastButtonState = reading;
}

// Method to check if a short press has been detected
bool Button::isShortPress() {
  if (shortPressDetected) {
    shortPressDetected = false;
    return true;
  }
  return false;
}

// Method to check if a long press has been detected
bool Button::isLongPress() {
  if (longPressDetected) {
    longPressDetected = false;
    return true;
  }
  return false;
}

// Method to check if an extra long press has been detected
bool Button::isExtraLongPress() {
  if (extraLongPressDetected) {
    extraLongPressDetected = false;
    return true;
  }
  return false;
}
