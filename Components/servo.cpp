// File will do Servo Turns and Init the servo
#include "defines.h"

#define MIDDLE 90
#define LEFT 0
#define RIGHT 180

volatile static uint8_t update_pwm_ready = 0;
volatile uint8_t count;
ISR(TIMER2_OVF_vect){
  
  update_pwm_ready = 0;
}

ISR(TIMER2_COMPA_vect){

}

void timer_2_servo_setup(void){
  
  
  //TCCR2A = (0b10 << COM1A0) | (0b00 << COM1B0) | (0b10 << WGM10);
//   TCCR2 = 0x0d;

//   TIMSK2 = (1 << TOIE2) | (1 << OCIE2A);

//   OCR2A = 255;
  
  //TCCR2B = (0b11 << WGM12) | (0b010 << CS10);
}

static void update_pwm(uint16_t i){
  update_pwm_ready = 1;
  while(update_pwm_ready != 0);
  OCR2A = i;
}

void servo_set(uint16_t deg,uint16_t max_deg){
  
  float set = (float)deg / (float)max_deg;
  set = (((float)SERVO_MAX-(float)SERVO_MIN)*set) + (float)SERVO_MIN;
  uint16_t piont = (uint16_t)set;
  update_pwm(piont);
  
}