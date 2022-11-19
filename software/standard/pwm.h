#ifndef PWM_H_
#define PWM_H_
#define N_1 (_BV(CS00))
#define N_8 (_BV(CS01))
#define N_64 (_BV(CS01) | _BV(CS00))
#define N_256 (_BV(CS02))
#define N_1024 (_BV(CS02) | _BV(CS00))

static void pwm_init(void) {
  //DDRB |= _BV(PB0);                   // set PWM pin as OUTPUT
  TCCR0B |= (1 << CS00);  //set prescaler
  TCCR0A |= _BV(WGM01) | _BV(WGM00);  // set timer mode to FAST PWM
  TCCR0A |= _BV(COM0A1) | _BV(COM0A0) | (1 << COM0B1) | (1 << COM0B0);  // connect Inverted PWM signal to pin OC0B and 0C0A
}

static void pwm_set_frequency(uint32_t N) {
  TCCR0B = (TCCR0B & ~((1 << CS02) | (1 << CS01) | (1 << CS00))) | N;  // set prescaler
}

static void pwm_set_duty1inv(uint8_t duty) {
  OCR0A = 255 - duty;  // set the OCRnx
}

static void pwm_set_duty2inv(uint8_t duty) {
  OCR0B = 255 - duty;  // set the OCRnx
}

static void pwm_stop(void) {
  TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));  // stop the timer
}
#endif