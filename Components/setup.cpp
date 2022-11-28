#include "defines.h"
void pins_init()
{
  //DDRC = (1<<DATA_PIN);   //Sets only A0 as INPUT
  DDRB = ((1<<LED_PIN) | (1<<TRIG_PIN) | (1 << NANO_LED_PIN)); // LED_PIN and TRIG_PIN as output
  DDRB |= (1 << SERVO_PIN);
  DDRD = (0<<ECHO_PIN); //ECHO_PIN as input
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
}

void uart_init() { // TX and RX init with IRQ
  UBRR0H = (uint8_t)((UBRR)>>8); // Set the UART speed as defined by UBRR
  UBRR0L = (uint8_t)UBRR;
  UCSR0B|=(1<<TXCIE0)|(1<<TXEN0); // Enable TX and TX IRQ.
  UCSR0B|=(1<<RXCIE0)|(1<<RXEN0); // Enable RX and RX IRQ 
  UCSR0C=(3<<UCSZ00); // Asynchronous UART, 8-N-1
}// end UART init