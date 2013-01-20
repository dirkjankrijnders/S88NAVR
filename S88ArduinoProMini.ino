//#define FCPU 8000000
#define LOAD INT1

#include <avr/interrupt.h>

volatile uint16_t buffer = 43690;
uint16_t msb = (1 << 15);
bool t;
uint8_t m = 0;

ISR(INT0_vect) {
  cli();
  if (((PIND & 0b00000100) == 0b00000100)) {   // If rising edge, read S88DATAIN
    t = (PIND & 16) ? msb : 0;
    //Serial.println(t);
    buffer |= t;
  } else {   // If falling edge, write S88DATAOUT
    if ((buffer & 1) == 1) {
      PORTD |= 0b11111111; //(1 << 6);
    } else {
      PORTD &= 0b11011111;
    }
    // Shift register and add value;
    buffer = (buffer >> 1);// | t;
  };
  
 
  sei();
}

ISR(INT1_vect) { // LOAD
  cli();
  buffer = 43690;
//  Serial.print('i');
  sei();
}

void setup() {
  Serial.begin(115200);
//  Serial.println("Hallo!");

//  pinMode(13, OUTPUT);
//  pinMode(5, INPUT);
  DDRB |= (1 << 5); // Arduino Pro Mini pin 13 as output
  DDRD = 0b00100010; // Arduino Pro Mini TX and pin 6 as output 
  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logical change
  EIMSK |= (1 << INT0);     // Turns on INT0
  EICRA |= (1 << ISC10);    // set INT1 to trigger rising edge
  EICRA |= (1 << ISC11);    
  EIMSK |= (1 << INT1);     // Turns on INT1
  sei();
  PORTB |= (1 << 5); // Arduino Pro Mini pin 13 HIGH
};  

void loop(){
//delay(300);
//  PORTB ^= (1 << 5); // Arduino Pro Mini pin 13 Toggle
};

