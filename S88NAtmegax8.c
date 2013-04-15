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
#include <util/delay.h>
#include <util/atomic.h>

volatile uint16_t buffer = 43690;
volatile uint16_t buf0, buf1, buf2;

ISR(INT0_vect) {
	if (((PIND & (1 << S88CLK)) == (1 << S88CLK))) {   // If rising edge, write S88DATAOUT
		if ((buffer & 1) == 0)
			PORTD &= ~(1 << S88DATAOUT);
		else 
			PORTD |= (1 << S88DATAOUT); //(1 << 6);

		// Shift register and add value;
		buffer = (buffer >> 1);// | t;
	} else {   // If falling edge, read S88DATAIN
		uint16_t t = (PINB & (1 << S88DATAIN)) ? 1: 0;
		buffer |= (t << 15);
		PORTB ^= (1 << LED);
	};

}

ISR(INT1_vect) { // LOAD
	buffer = ~(buf2 & buf1 & buf0);
//	buffer = 43691;
}

ISR(TIMER1_COMPA_vect) {
	buf0 = buf1;
	buf1 = buf2;
	buf2 = 0;
	buf2 |= (PIND >> 4); // The four msb of D become the lsb of the buffer
	buf2 |= (PINB << 4); // All bits of B become the 5-10 bits of the buffer
	buf2 &= ~((1 << 11)|(1 << 12));
	buf2 |= (PINC << 10); // The lsb of C become the 11-16 bits of the buffer	
	
	//buffer = 43691;
}

int main(){
	DDRD = 0; // Everthing 
	DDRB = 0; // is input
	DDRC = 0; // except... 
	DDRD |= (1 << S88DATAOUT); // Is the only output...
	DDRB |= (1 << LED); // LED is output as well...
	PORTC = 0xFF;
	PORTB = 0xFF;
	PORTD = 0xF0;
	EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logical change
	EIMSK |= (1 << INT0);     // Turns on INT0
	EICRA |= (1 << ISC10);    // set INT1 to trigger rising edge
	EICRA |= (1 << ISC11);    
	EIMSK |= (1 << INT1);     // Turns on INT1
	buffer = 0;//~(43690);
	
	TIMSK1 = _BV(OCIE1A); // Interrupt on T0CNT == COMPA
	TCCR1B = _BV(WGM12);  // Timer clear on Compare match
	OCR1A = 200;
	 TCCR1B |= (1<<CS00)|(1<<CS01) ;
	sei();
	while(1) {
/*		_delay_ms(25.0);
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		}*/
	};
	return 0;
};

