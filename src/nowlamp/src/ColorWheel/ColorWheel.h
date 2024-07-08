#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <Arduino.h>

#define NUM_COLORS 7

class ColorWheel {
   public:
    // Constructor to initialize the color wheel with the number of steps
    ColorWheel();

    // Method to increment the color wheel
    void increment();
    void decrement();
    void setColor(int color);

    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();
    uint8_t getWhite();

   private:
    /*
    // red
    // yellow
    // orange
    // green
    // blue
    // purple
    // white
    */

    uint8_t colors[NUM_COLORS][4] = {
        {0, 0, 0, 0},   {255, 0, 0, 0}, {0, 255, 0, 0}, {0, 0, 255, 0},
        {0, 0, 0, 255}, {255, 255, 0, 0}, {255, 255, 255, 255}};

    int currentStep;  // Current step of the color wheel
};

#endif  // COLORWHEEL_H