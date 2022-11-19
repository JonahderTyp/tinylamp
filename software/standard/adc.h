#ifndef ADC_H_
#define ADC_H_

void adc_enable() {
  ADCSRA |= (1 << ADEN);
}
void adc_disable() {
  ADCSRA &= ~(1 << ADEN);
}

void adc_init() {
  ADMUX |= (1 << MUX0);                                  //PB2
  ADMUX &= ~(1 << ADLAR);                                // right adjust

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  //Prescaler set to 64
  adc_enable();
}

void inline adc_startConversion() {
  // Start the conversion
  ADCSRA |= (1 << ADSC);
}

uint16_t inline adc_getReading() {
  return ADCL | ((ADCH & 0x03) << 8);
}

void inline adc_wait() {
  // Wait for it to finish
  while (ADCSRA & (1 << ADSC))
    ;
}

uint16_t adc_readB(void) {
  adc_startConversion();
  adc_wait();
  return adc_getReading();
}


#endif