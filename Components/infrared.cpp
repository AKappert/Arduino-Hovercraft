#include "defines.h"

long time;
int ir_dist;
int ir_range;
long return_ir_time(){return time;}
int return_ir_dist(){return ir_dist;}
int return_ir_range(){return ir_range;};

uint8_t analog_reference = DEFAULT;

uint16_t readFromPin(uint8_t pin)
{
    uint8_t low, high;
    if (pin >= 14) pin -= 14; // allow for channel or pin numbers
    ADMUX = (analog_reference << 6) | (pin & 0x07);
    sbi(ADCSRA, ADSC);
    while (bit_is_set(ADCSRA, ADSC));
    low  = ADCL;
    high = ADCH;
    return (high << 8) | low;
}

void writeIRToLED(int ir_dist)
{  
    if(ir_dist > MAX_RANGE)
    {
      ir_range = 255;
      PORTB |= (1<<NANO_LED_PIN);
      _delay_us(500);
    }

    else if(ir_dist < MIN_RANGE)
    {
      ir_range = 0;
      PORTB |= (1<<NANO_LED_PIN);
    }
    else
    {
      ir_range = (ir_dist-MIN_RANGE)*10.2;
      PORTB &= ~(1<<NANO_LED_PIN);
    }
    OCR2A = ir_range;
    OCR2B = ir_range;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int read_IR_Sensor()
{
  int result = readFromPin(DATA_PIN);
  float volts = result * 0.004882812;
  ir_dist = 13*pow(volts, -1)* 2.54 -3;
  return ir_dist;
}

void print_IR_distance(int ir_dist)
{
  Serial.print("ir_dist :");
  Serial.println(ir_dist);
}


