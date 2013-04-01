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

volatile uint16_t buffer = 43690;

enum deb_state{
	DEB_OCC,
	DEB_OF,
	DEB_FREE,
	DEB_FO
} ;

struct deb {
	enum deb_state state;
	uint8_t count;
} deb[16];

ISR(INT0_vect) {
	if (((PIND & (1 << S88CLK)) == (1 << S88CLK))) {   // If rising edge, write S88DATAOUT
		if ((buffer & 1) == 1)
			PORTD &= ~(1 << S88DATAOUT);
		else 
			PORTD |= (1 << S88DATAOUT); //(1 << 6);

		// Shift register and add value;
		buffer = (buffer >> 1);// | t;
	} else {   // If falling edge, read S88DATAIN
		uint16_t t = (PINB & (1 << S88DATAIN)) ? 0 : 1;
		buffer |= (t << 15);
		PORTB ^= (1 << LED);
	};

}

ISR(INT1_vect) { // LOAD
	PORTD &= ~(1 << S88DATAOUT);
	buffer = 0;//~(43690);
	buffer = deb[8].count;
//	deb[8].count = 0;
	uint8_t ii;
	for (ii = 0; ii < 16; ii++) {
//		buffer |= ((deb[ii].state == DEB_FREE) | (deb[ii].state == DEB_FO)) ? (1 << ii) : 0;
/*		if (deb[ii].count == 0xff) {
			buffer |= (1 << (ii));
		} else {
			buffer &= ~((1 << (ii)) & buffer);
		};
*/
	}
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
	uint16_t temp;
	uint16_t mask;
	uint8_t ii;
	while(1) {
		temp = 0;
		_delay_ms(0.2);
		temp |= (PIND >> 4); // The four msb of D become the lsb of the buffer
		temp |= (PINB << 4); // All bits of B become the 5-10 bits of the buffer
		temp &= ~((1 << 11)|(1 << 12));
		temp |= (PINC << 10); // The lsb of C become the 11-16 bits of the buffer
		temp |= (PINC << 10);
		mask = 1;
		for (ii = 0; ii < 16; ii++) {
			deb[ii].count = (deb[ii].count << 1);
			if (((temp & mask) == mask)) {
				deb[ii].count |= 1;
			} else {
				deb[ii].count |= 0;
				
/*				if (deb[ii].count > 0) deb[ii].count--;
				if ((deb[ii].state == DEB_OF) & (deb[ii].count < 55))  {
					deb[ii].count = 0;
					deb[ii].state = DEB_FO;
				}
			} else {
				if (deb[ii].count < 255) deb[ii].count++;
				if ((deb[ii].state == DEB_FO) & (deb[ii].count > 200))  {
					deb[ii].count = 255;
					deb[ii].state = DEB_OF;
				}*/
			}	
			mask = (mask << 1);
		}
	};
	return 0;
};

