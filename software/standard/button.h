#include <Arduino.h>
//#ifndef BUTTON_H_
//#define BUTTON_H_
#include "adc.h"


class button {
private:
  const uint16_t presicion = 10;
  const uint16_t vals[10] = {
    252,
    372,
    485,
    501,
    622,
    657,
    708,
    737,
    755,
    796
  };

  const int readings = 10;

  int8_t lastButton = -1;

  int8_t decodeButton(uint16_t reading) {
    for (uint8_t i = 0; i < 10; i++) {
      if (reading >= (vals[i] - presicion) && reading <= (vals[i] + presicion)) {
        return i;
      }
    }
    return -1;
  }

public:

  button() {
    adc_init();
  }

  uint8_t hasNewReading(){
    return (lastButton != -1);
  }

  uint8_t getReading(){
    return lastButton;
  }

  void handle() {
    uint16_t smooth = 0;
    for (uint8_t i = 0; i < readings; i++) {
      smooth += adc_read();
    }
    smooth = smooth / readings;

    lastButton = decodeButton(smooth);
  }
};

//#endif