#ifndef AIRSETTINGS_H
#define AIRSETTINGS_H

#include <Arduino.h>

class AirSettings {
 public:
  AirSettings();

  bool buttonsEnabled = true;
  int colorWheelOverride = -1;
  int mode = 0;

 private:
}

#endif  // AIRSETTINGS_H