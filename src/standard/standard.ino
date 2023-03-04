#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Arduino.h>
#include "pwm.h"
#include "aled.h"
#include "colors.h"
#include "button.h"


#define LEDW (PB0)
#define LEDA (PB1)
#define SWIN (PB2)
#define ALED (PB3)
#define ALEDEN (PB4)

#define PIXEL_NUM (8)

uint64_t quartMillis = 0;

// Timer interupt for runtime calc
ISR(TIM0_OVF_vect) {
  quartMillis++;
  //PORTB ^= _BV(LEDW); // toggle LED pin
}

//ISR(INT0_vect)

struct pixel {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} pixels[PIXEL_NUM];

void inline enableLed() {
  PORTB |= _BV(ALEDEN);
  PORTB &= ~_BV(ALED);
  _delay_ms(10);
}

void inline disableLed() {
  PORTB &= ~_BV(ALEDEN);
  PORTB |= _BV(ALED);
}




int main(void) {
  DDRB |= _BV(LEDW) | _BV(LEDA) | _BV(ALED) | _BV(ALEDEN);
  PORTB |= (_BV(LEDW) | _BV(LEDA) | _BV(ALEDEN));

  //Aled
  enableLed();
  //struct pixel p = { 0, 10, 0 };

  //mode
  uint8_t mode = 0;

  //pwm
  uint8_t ch0 = 0, ch1 = 0;
  pwm_init();
  pwm_set_frequency(N_8);
  pwm_set_duty0inv(ch0);
  pwm_set_duty1inv(ch1);

  //adc
  //adc_init();

  //flashing
  TIMSK0 |= _BV(TOIE0);  // enable Timer Overflow interrupt
  sei();                 // enable global interrupts
  uint8_t flashcounter = 0;
  uint64_t lastflash = 0;
  uint16_t doubleinterval = 40;  //~10ms
  struct pixel color = { 0, 0, 0 };


  /* loop */
  while (1) {

    mode = 5;
    color = { RED };

    if (quartMillis - lastflash >= doubleinterval) {
      lastflash = quartMillis;
      flashcounter++;
      //PORTB |= _BV(ALEDEN);
    }

    //PORTB &= ~_BV(ALEDEN);



    switch (mode) {
      case 5:
        for (int i = 0; i < PIXEL_NUM; ++i)
          pixels[i] = color;
        ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);
        break;
      case 6:
        switch (flashcounter) {
          case 0:
            for (int i = 0; i < PIXEL_NUM / 2; ++i)
              pixels[i] = { RED };
            for (int i = PIXEL_NUM / 2; i < PIXEL_NUM; ++i)
              pixels[i] = { BLACK };
            break;
          case 5:
            for (int i = 0; i < PIXEL_NUM / 2; ++i)
              pixels[i] = { BLACK };
            for (int i = PIXEL_NUM / 2; i < PIXEL_NUM; ++i)
              pixels[i] = { RED };
            break;
          case 10:
            flashcounter = -1;
            break;
        }
        ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);
        break;
      case 7:
        switch (flashcounter) {
          case 0:
          case 9:
          case 18:
            color = { ORANGE };
            break;
          case 7:
          case 16:
          case 25:
            color = { 0, 0, 0 };
            break;
          case 42:
            flashcounter = -1;
            break;
            break;
        }
        for (int i = 0; i < PIXEL_NUM; ++i)
          pixels[i] = color;
        ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);
        break;
    }


    //ALED Turn off
    /*PORTB &= ~_BV(ALED);
    PORTB |= _BV(ALEDEN);
    _delay_ms(5);

    for (int i = 0; i < PIXEL_NUM; ++i)
      pixels[i] = { 255, 255, 255 };
    ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);

    _delay_ms(100);

    PORTB |= _BV(ALED);
    PORTB &= ~_BV(ALEDEN);

    _delay_ms(200);*/

    //uint16_t data = map(adc_read(),0,1023,0,255);
    //uint16_t data = adc_readB();
    //data = 0xFFFF;
    //data++;
    //uint16_t data = 0x03FF;


    //duty++;

    //if(duty == 255){
    //pwm_stop();
    //}

    //p.r = duty;
    //p.g = duty;
    //p.b = duty;

    /*for (int i = 0; i < PIXEL_NUM; ++i) {
      if ((countr >> i) & 0x01) {
        pixels[i] = pixel{ 0, 20, 0 };
      } else {
        pixels[i] = pixel{ 0, 0, 0 };
      }
    }

    ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);*/

    //countr = 1;

    //PORTB |= ((halfMillis & 0x01) << ALEDEN);
    //PORTB &= ((halfMillis & 0x01) << ALEDEN);

    //PORTB &= (1 << _BV(ALEDEN));

    /*for (int i = 0; i < 2; i++) {

      for (int i = 0; i < PIXEL_NUM; ++i)
        pixels[i] = pixel{ 0,0, 0 };
        //pixels[i] = pixel{ ORANGE };
      ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);
      _delay_ms(70);
      for (int i = 0; i < PIXEL_NUM; ++i)
        pixels[i] = pixel{ 0, 0, 0 };
      ws2812_setleds((struct cRGB *)pixels, PIXEL_NUM);
      _delay_ms(20);
    }*/
    //PORTB |= _BV(ALEDEN);

    //_delay_ms(20);
    //PORTB &= ~_BV(ALEDEN);
    //_delay_ms(150);
  }
}