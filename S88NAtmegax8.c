//#define F_CPU 8000000UL

#define S88LOAD PD3 // INT1
#define S88CLK PD2
#define S88RST // Unused
#define S88DATAIN PB7
#define S88DATAOUT PD0
#define LED PB6 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t buffer = 43690;
uint16_t msb = (1 << 15);
uint8_t t;
uint8_t m = 0;

ISR(INT0_vect) {
//    PORTD ^= 0xFF;
//  cli();
  if (((PIND & (1 << S88CLK)) == (1 << S88CLK))) {   // If rising edge, write S88DATAOUT
    if ((buffer & 1) == 1) {
      PORTD |= (1 << S88DATAOUT); //(1 << 6);
    } else {
      PORTD &= ~(1 << S88DATAOUT);
    }
    // Shift register and add value;
    buffer = (buffer >> 1);// | t;
  } else {   // If falling edge, read S88DATAIN
    t = (PINB & S88DATAIN) ? 1 : 0;
    buffer |= (t << 15);
  };
 
//  sei();
}

ISR(INT1_vect) { // LOAD
  buffer = 0b1100111011110000;
  PORTB ^= (1 << LED);
/*  buffer = 0;
  buffer |= (PIND >> 4); // The four msb of D become the lsb of the buffer
  buffer |= (PINB << 4); // All bits of B become the 5-10 bits of the buffer
  buffer |= (PINC << 10); // The lsb of C become the 11-16 bits of the buffer
*/
//  buffer = (PINC << 8) | PINB;
}

int main(){
  DDRD |= (1 << S88DATAOUT); // Is the only output...
  DDRB |= (1 << LED); // LED is output as well...
//  DDRB = (1 << 5);
//  DDRC = 0;
//  DDRD = 0b00100010; // Arduino Pro Mini TX and pin 6 as output 
  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logical change
  EIMSK |= (1 << INT0);     // Turns on INT0
  EICRA |= (1 << ISC10);    // set INT1 to trigger rising edge
  EICRA |= (1 << ISC11);    
  EIMSK |= (1 << INT1);     // Turns on INT1
  sei();
  while(1) {
//    PORTB ^= (1 << 5);
  };
  return 0;
};

