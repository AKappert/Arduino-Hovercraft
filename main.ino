#include "Components/defines.h"




//  ======================================== VARIABLES ========================================
  
  
  // Enums
  enum MACHINE_STATE {SHUTDOWN, CALCULATING, MOVING, PAUSING};
  enum LED_TYPE {PWM, DISCRETE};
  enum SERVO_STATE {STATIC, MOVING_CW, MOVING_CCW};

// Flags structure
  volatile struct {
  uint8_t TX_finished:1;
  uint8_t sample:1;
  uint8_t mode:1;
  uint8_t stop:1;
  uint8_t T1_ovf0:2;
  uint8_t T1_ovf1:2;
  } flags;

// Pulse data structure
static volatile struct {
  uint16_t pulse0;
  uint16_t pulse1;
//  uint16_t pulse3;
  uint16_t t_start0;
  uint16_t t_end0;
  uint16_t t_start1;
  uint16_t t_end1;
} PULSE_data; 






//  ======================================== FUNCTIONS ========================================

// Function used to set servo to a int value from 0-180
void setServoAngle(int deg) {

}

// Function used to initialize
void gpio_init() {
  cli(); //Stop Interrupts
  DDRD = (0<<ECHO_PIN); //ECHO_PIN as input
  DDRB = ((1<<LED_PIN) | (1<<TRIG_PIN)); // LED_PIN and TRIG_PIN as output
  sei(); //Allow Interrupts
}


//  ======================================== SETUP ========================================
void setup() {
//  (  1 ) Divide servo's 180deg range of motion into 180 / SERVO_STEP
//  (  2 ) Create array or datatype (ie an object class) to store each angle.
//          -> min/max heaps could be a good idea
//  (  3 ) Setup servo pins
//  (  4 ) Setup servo motion
//  (  5 ) Setup timers
//  (  6 ) Setup US sensor pins
//  (  7 ) 
//  (  8 ) 
//  (  9 ) 
//  ( 10 ) 
//  ( 11 ) 
}


//  ======================================== MAIN ========================================
void loop() {
//  (  1 ) Check if IS_RUNNING = true.
//  (  2 ) For each step in servo's range of motion, measure distance
//  (  3 ) Store distance and servo angle into array/datatype
//  (  4 ) Start a loop that cycles through each value in the array/datatype
//  (  5 ) Average values next to each other in sizes of SERVO_CLUSTER_SIZE
//  (  6 ) Calculate what moves need to be done to move the hover craft
//  (  7 ) 
//  (  8 ) 
//  (  9 ) 
//  ( 10 ) 
//  ( 11 ) 

}
