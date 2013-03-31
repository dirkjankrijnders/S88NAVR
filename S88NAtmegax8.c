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


volatile uint16_t buffer[2] = {43690, 0x00FF};
volatile uint8_t oBuf = 0;
volatile uint8_t rBuf = 1;

ISR(INT0_vect) {
	if (((PIND & (1 << S88CLK)) == (1 << S88CLK))) {   // If rising edge, write S88DATAOUT
		if ((buffer[oBuf] & 1) == 1)
			PORTD &= ~(1 << S88DATAOUT);
		else 
			PORTD |= (1 << S88DATAOUT); //(1 << 6);

		// Shift register and add value;
		buffer[oBuf] = (buffer[oBuf] >> 1);// | t;
	} else {   // If falling edge, read S88DATAIN
		uint16_t t = (PINB & (1 << S88DATAIN)) ? 0 : 1;
		buffer[oBuf] |= (t << 15);
		PORTB ^= (1 << LED);
	};

}

ISR(INT1_vect) { // LOAD
	PORTD &= ~(1 << S88DATAOUT);

	if (oBuf == 0) {
		oBuf = 1;
		rBuf = 0;
	} else {
		oBuf = 0;
		rBuf = 1;
	};
	buffer[rBuf] = 0;
	buffer[rBuf] |= (PIND >> 4); // The four msb of D become the lsb of the buffer[oBuf]
	buffer[rBuf] |= (PINB << 4); // All bits of B become the 5-10 bits of the buffer[oBuf]
	buffer[rBuf] &= ~((1 << 11)|(1 << 12));
	buffer[rBuf] |= (PINC << 10); // The lsb of C become the 11-16 bits of the buffer[oBuf]
	buffer[rBuf] |= (PINC << 10);
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
	sei();
	while(1) {
	};
	return 0;
};

