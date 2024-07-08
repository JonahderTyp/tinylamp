#include "ColorWheel.h"

// Constructor implementation
ColorWheel::ColorWheel() {}

// Method to increment the color wheel
void ColorWheel::increment() { currentStep = (currentStep + 1) % NUM_COLORS; }

void ColorWheel::decrement() {
  if (currentStep == 0) {
    currentStep = NUM_COLORS - 1;
  } else {
    currentStep = (currentStep - 1) % NUM_COLORS;
  }
}

void ColorWheel::setColor(int color) {
  currentStep = color;
}

uint8_t ColorWheel::getRed() {
  return colors[currentStep][0];
}

uint8_t ColorWheel::getGreen() {
  return colors[currentStep][1];
}

uint8_t ColorWheel::getBlue() {
  return colors[currentStep][2];
}

uint8_t ColorWheel::getWhite() {
  return colors[currentStep][3];
}