#define F_CPU 8000000UL
#define LOAD INT1

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t buffer = 43690;
uint16_t msb = (1 << 15);
uint8_t t;
uint8_t m = 0;

ISR(INT0_vect) {
  cli();
  if (((PIND & 0b00000100) == 0b00000100)) {   // If rising edge, write S88DATAOUT
    if ((buffer & 1) == 1) {
      PORTD |= 0b11111111; //(1 << 6);
    } else {
      PORTD &= 0b11011111;
    }
    // Shift register and add value;
    buffer = (buffer >> 1);// | t;
  } else {   // If falling edge, read S88DATAIN
    t = (PIND & 16) ? msb : 0;
    buffer |= (t << 15);
  };
  
 
  sei();
}

ISR(INT1_vect) { // LOAD
  buffer = 0b1100111011110000;
//  buffer = (PINC << 8) | PINB;
}

int main(){
  DDRB = (1 << 5);
  DDRC = 0;
  DDRD = 0b00100010; // Arduino Pro Mini TX and pin 6 as output 
  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logical change
  EIMSK |= (1 << INT0);     // Turns on INT0
  EICRA |= (1 << ISC10);    // set INT1 to trigger rising edge
  EICRA |= (1 << ISC11);    
  EIMSK |= (1 << INT1);     // Turns on INT1
  sei();
  while(1) {
    PORTB ^= (1 << 5);
  };
  return 0;
};

