// INCLUDES
#include "Arduino.h"
#include "avr/interrupt.h"
#include "stdlib.h"
#include "avr/io.h"
#include <Wire.h>
//#include "us_measurement.h"
#include "us_manager.h"

// PIN DEFINITIONS
// LED PINS
#define LED_PIN PB3 
#define NANO_LED_PIN PB5



// INFRARED SENSOR PINS
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define DATA_PIN A0

#define F_CPU 16000000UL
#define BAUD 9600UL
#define UBRR ((F_CPU)/((BAUD)*(16UL))-1)
#define SCL_CLOCK 100000L

#define PWM_TOP 2500

#define MIN_RANGE 15
#define MAX_RANGE 40








// void pins_init()
// {
//   DDRC = (1<<DATA_PIN);   //Sets only A0 as INPUT
//   DDRB = ((1<<LED_PIN) | (1<<TRIG_PIN) | (1 << NANO_LED_PIN)); // LED_PIN and TRIG_PIN as output
//   DDRD = (0<<ECHO_PIN); //ECHO_PIN as input
//   TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
//   TCCR2B = _BV(CS22);
// }



// //Function used to initialize pins as input or output and set high or low
// void gpio_init() {
//   cli(); //Stop Interrupts

//   sei(); //Allow Interrupts
// } //end gpio_init

// void uart_init() { // TX and RX init with IRQ
//   UBRR0H = (uint8_t)((UBRR)>>8); // Set the UART speed as defined by UBRR
//   UBRR0L = (uint8_t)UBRR;
//   UCSR0B|=(1<<TXCIE0)|(1<<TXEN0); // Enable TX and TX IRQ.
//   UCSR0B|=(1<<RXCIE0)|(1<<RXEN0); // Enable RX and RX IRQ 
//   UCSR0C=(3<<UCSZ00); // Asynchronous UART, 8-N-1
// }// end UART init